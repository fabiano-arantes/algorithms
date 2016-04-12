#include <bintrie.h>
#include <heapsort.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int (* function)(mem* const , mem* const);


static void write_trie_buffer(mem* buffer, const bin_trie_node* const node)
{
    if ((node->childs[0] == NULL) && (node->childs[1] == NULL))
    {
        MEM_APPEND_BIT(buffer, BIT_1);

        MEM_APPEND_VALUE(buffer, &node->c);

        return;
    }

    MEM_APPEND_BIT(buffer, BIT_0);

    write_trie_buffer(buffer, node->childs[0]);
    write_trie_buffer(buffer, node->childs[1]);
}

static int huffman_compress(mem* const buffer_r, mem* const buffer_w)//, const size_t bytes_input)
{
    mem trie_buf;
    mem buf_data;
    size_t n, node_count;
    size_t bytes_total; // size of output buffer. Temporary value used only when writing output file
    size_t size;
    size_t count[UCHAR_MAX + 1] = {0}; // counter of char occurrences
    bin_trie_node *node1, *node2, *trie_root;
    sort_data heap;
    bin_trie_st st[UCHAR_MAX + 1];
    unsigned char c;

    memset(&st, 0, sizeof(st));
    memset(&heap, 0, sizeof(sort_data));
    memset(&trie_buf, 0, sizeof(trie_buf));
    memset(&buf_data, 0, sizeof(buf_data));

    //set comparison function: freq ascending
    heap.cmp_func = asc_cmp_trie_freq;

    mem_seek(buffer_r, SEEK_READ_RESET);

    //count chars
    while (MEM_GET_NEXT_VALUE(buffer_r, &c))
    {
        ++count[(size_t) c];
    }

#ifdef DEBUG
    for (n = 0; n <= UCHAR_MAX; ++n)
    {
        if (count[n] > 0)
        {
            MSG_ARG("[%zu] '%c' = %zu", n, (unsigned char) n, count[n]);
        }
    }
#endif

    node_count = 0;

    for (n = 0; n <= UCHAR_MAX; ++n)
    {
        if (count[n] > 0)
        {
            ++node_count;

            //create trie node
            node1 = trie_new_node(n, count[n], NULL, NULL);

            //insert node in the heap, in order to sort the nodes in order of frequency
            heap_put(&heap, node1);
        }
    }

    //remove two nodes with the lowest freq and merge them
    while ((node1 = heap_remove_top(&heap)) != NULL)
    {
        if ((node2 = heap_remove_top(&heap)) == NULL)
        {
            break;
        }

        node1 = trie_merge(node1, node2);

        heap_put(&heap, node1);
    }

    //save trie root
    trie_root = node1;

    trie_symbol_table(trie_root, st);

#ifdef DEBUG

    print_symbol_table(st);
#endif

    n = mem_size(buffer_r);

    mem_seek(buffer_r, SEEK_READ_RESET);

    for(size_t i = 0; i < n; ++i)
    {
        if (MEM_GET_NEXT_VALUE(buffer_r, &c))
        {
            mem_append_bits_and_shift(&buf_data, &st[c].bits, st[c].bits_count);
        }
    }

    MSG_ARG("bits_output = %lu", mem_size_in_bits(&buf_data));

    size_t bytes_output = mem_size(&buf_data);

    //write trie in buffer
    write_trie_buffer(&trie_buf, trie_root);

    //get total buffer size
    //bytes_total = sizeof(bytes_input) + sizeof(bytes_output) + (bytes_trie + 1) + bytes_output;
    //             EXPANDED SIZE   COMPRESSED DATA SIZE          TRIE             OUTPUT
    bytes_total = sizeof(size_t) + sizeof(bytes_output) + mem_size(&trie_buf) + bytes_output;

    //write input size in output buffer
    size = mem_size(buffer_r);
    MEM_APPEND_VALUE(buffer_w, &size);

    //write size of compressed data in output buffer
    MEM_APPEND_VALUE(buffer_w, &bytes_output);

    //write trie data in output buffer
    mem_append_bits_and_shift(buffer_w, trie_buf.mem, mem_size_in_bits(&trie_buf));

    //copy compressed data to buffer
    mem_append_bits_and_shift(buffer_w, buf_data.mem, mem_size_in_bits(&buf_data));

    trie_clean(trie_root);

    return 1;
}

static int huffman_expand(mem* const buffer_r, mem* const buffer_w)
{
    bin_trie_st st[UCHAR_MAX + 1];
    bin_trie_node *trie_root, *trie_node = NULL;
    size_t original_size, compressed_size;
    bit_t b;

    memset(&st, 0, sizeof(st));

    if(MEM_GET_NEXT_VALUE(buffer_r, &original_size) == 0)
    {
        return 0;
    }

    if(MEM_GET_NEXT_VALUE(buffer_r, &compressed_size) == 0)
    {
        return 0;
    }

    trie_root = read_bin_trie(buffer_r);

    trie_symbol_table(trie_root, st);

    print_symbol_table(st);

    trie_node = trie_root;

    while ((trie_node != NULL) && (original_size > 0))
    {
        if ((trie_node->childs[0] == NULL) && (trie_node->childs[1] == NULL))
        {
            MEM_APPEND_VALUE(buffer_w, &trie_node->c);

            trie_node = trie_root;

            --original_size;
        }
        else
        {
            if (MEM_GET_NEXT_BIT(buffer_r, &b))
            {
                trie_node = trie_node->childs[b];
            }
            else
            {
                break;
            }
        }
    }

    return 1;
}

int main(int argc, char *argv[])
{
    FILE *fp;
    size_t n;
    mem buffer_r, buffer_w;
    char *file_input, *file_output;

    memset(&buffer_r, 0, sizeof(buffer_r));
    memset(&buffer_w, 0, sizeof(buffer_w));

    if ((argc < 3) || ((strcmp(argv[1], "-e") == 0) && (argc < 4)))
    {
        printf("Usage:\n");
        printf("\t%s <file input> <file output> (compress)\n", argv[0]);
        printf("\t%s -e <file input> <file output> (expand)\n", argv[0]);
        return 1;
    }

    if ((strcmp(argv[1], "-e") == 0))
    {
        file_input = argv[2];
        file_output = argv[3];
        function = &huffman_expand;
    }
    else
    {
        file_input = argv[1];
        file_output = argv[2];
        function = &huffman_compress;
    }

    fp = fopen(file_input, "rb");
    if (fp == NULL)
    {
        printf("Fail to open file '%s'", file_input);
    }

    //go to end of file
    fseek(fp, 0L, SEEK_END);

    //get file size
    const size_t buffer_size = ftell(fp);

    MSG_ARG("buffer_size = %zu", buffer_size);

    //back to beginning of file
    rewind(fp);

    //read all file
    n = mem_read_file(&buffer_r, fp, buffer_size);

    if (n != buffer_size)
    {
        printf("Error: there are chars left: bytes read = %zu, buffer size = %zu\n", n, buffer_size);
        return 1;
    }

    fclose(fp);

    if (function(&buffer_r, &buffer_w) == 0)
    {
        printf("Error on process file\n");
        return 1;
    }

    fp = fopen(file_output, "wb");
    if (fp == NULL)
    {
        printf("Error: fail to open file '%s'\n", file_output);
    }

    //write output with size stored in fist size_t value in buffer
    fwrite(buffer_w.mem, 1, mem_size(&buffer_w), fp);

    fclose(fp);

    //free(buffer_r);
    //free(buffer_w);

    return 0;
}
