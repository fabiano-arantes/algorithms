#include <bintrie.h>
#include <heapsort.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char* (* function)(unsigned char* const buffer_r, const size_t buffer_size);


static size_t write_trie(unsigned char** buffer, const bin_trie_node* const node)
{
    static size_t bit_count = 0;
    static size_t bytes = 1;
    static size_t bits_total = sizeof(**buffer) << 3;

    if ((node->childs[0] == NULL) && (node->childs[1] == NULL))
    {
        ++bit_count;
        **buffer |= (1 << (bits_total - bit_count));

        **buffer |= (node->c >> bit_count);

        ++(*buffer);

        **buffer = 0 | (node->c << (bits_total - bit_count));

        if (bit_count == bits_total)
        {
            ++(*buffer);
            **buffer = 0;
            bit_count = 0;
        }

        return ++bytes;
    }

    **buffer &= (0 << (8 - bit_count));
    ++bit_count;

    if (bit_count == bits_total)
    {
        ++(*buffer);
        **buffer = 0;
        bit_count = 0;

        ++bytes;
    }

    write_trie(buffer, node->childs[0]);
    write_trie(buffer, node->childs[1]);

    return bytes;
}

static unsigned char* huffman_compress(unsigned char* const buffer_r, const size_t bytes_input)
{
    unsigned char *buf_r, *buf_w; // pointers to read and write bytes in buffer
    size_t c, n, node_count;
    size_t bits_output; // size of compressed data
    size_t bytes_trie; // size of trie
    size_t bytes_total; // size of output buffer. Temporary value used only when writing output file
    size_t buffer_w_size;
    size_t count[UCHAR_MAX + 1] = {0}; // counter of char occurrences
    bin_trie_node *node1, *node2, *trie_root;
    sort_data heap;
    bin_trie_st st[UCHAR_MAX + 1];
    short n_bits;
    unsigned short total_bits;

    memset(&st, 0, sizeof(st));
    memset(&heap, 0, sizeof(sort_data));

    //set comparison function: freq ascending
    heap.cmp_func = asc_cmp_trie_freq;

    //count chars
    n = bytes_input;
    buf_r = buffer_r;
    do
    {
        ++count[(size_t) *(buf_r++)];
    }
    while(--n);

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

    n_bits = bits_output = 0;
    buf_r = buf_w = buffer_r;
    n = bytes_input;
    c = (size_t) *buf_r++;
    *buf_w = 0;
    total_bits = sizeof(*buf_w) << 3;

    do
    {
        bits_output += st[c].bits_count;

        if ((size_t)(n_bits + st[c].bits_count) < total_bits)
        {
            *buf_w |= st[c].bits << (total_bits - st[c].bits_count - n_bits);
            n_bits += st[c].bits_count;
        }
        else
        {
            *buf_w |= st[c].bits >> (n_bits + st[c].bits_count - total_bits);
            ++buf_w;

            *buf_w = 0 | (st[c].bits << ((total_bits << 1) - n_bits - st[c].bits_count));
            n_bits += st[c].bits_count - total_bits;
        }
        c = (size_t) *buf_r++;
    }
    while(--n);

    MSG_ARG("bits_output = %lu", bits_output);

    size_t full_bytes_output = (bits_output / (sizeof(*buf_w) << 3));
    unsigned char bits_offset = bits_output % (sizeof(*buf_w) << 3);
    size_t bytes_output = full_bytes_output + ((bits_offset > 0) ? 1 : 0);

    //output structure: (size_t) size of output buffer + (size_t) size of expanded data + (size_t) full bytes output + (char) bits offset + trie (estimative) + compressed data
    buffer_w_size = sizeof(bytes_total) + sizeof(bytes_input) + (sizeof(bytes_output) + sizeof(bits_offset)) + (node_count << 1) + bytes_output;
    unsigned char *buffer_w = malloc(buffer_w_size);

    if (buffer_w == NULL)
    {
        printf("Error on allocate output buffer\n");
        return NULL;
    }

    //skip reserved space for output buffer size
    buf_w = buffer_w + sizeof(bytes_total);

    //write input size in output buffer
    memcpy(buf_w, &bytes_input, sizeof(bytes_input));

    //shift
    buf_w += sizeof(bytes_input);

    //write size of compressed data in output buffer
    memcpy(buf_w, &bytes_output, sizeof(bytes_output));

    //shift
    buf_w += sizeof(bytes_output);

    //write size of bits offset of compressed data size
    memcpy(buf_w, &bits_offset, sizeof(bits_offset));

    //shift
    buf_w += sizeof(bits_offset);

    //write trie in buffer
    bytes_trie = write_trie(&buf_w, trie_root);

    buf_w += sizeof(*buf_w);

    //shift
    //buf_w += bytes_trie;

    //copy compressed data to buffer
    memcpy(buf_w, buffer_r, sizeof(*buffer_r) * bytes_output);

    //get total buffer size
    bytes_total = sizeof(bytes_input) + sizeof(bytes_output) + (bytes_trie + 1) + bytes_output;

    //write total byte at beginning of buffer
    memcpy(buffer_w, &bytes_total, sizeof(bytes_total));

    trie_clean(trie_root);

    return buffer_w;
}

static unsigned char* huffman_expand(unsigned char* const buffer_r, size_t buffer_r_size)
{
    const unsigned char* buf_r = buffer_r;
    unsigned char* buffer_w = NULL;
    unsigned char* buf_w = NULL;

    (void)buffer_r_size;

    bin_trie_st st[UCHAR_MAX + 1];
    bin_trie_node *trie_root, *trie_node = NULL;
    size_t original_size, compressed_size;
    unsigned char bits_offset;
    unsigned short bits_count = 0;

    memset(&st, 0, sizeof(st));

    original_size = *((size_t *)buf_r);

    buf_r += sizeof(size_t);

    compressed_size = *((size_t *)buf_r);

    buf_r += sizeof(size_t);

    bits_offset = *((typeof(bits_offset) *)buf_r);

    buf_r += sizeof(bits_offset);

    trie_root = read_bin_trie(&buf_r);

    buf_r += sizeof(*buf_r);

    trie_symbol_table(trie_root, st);

    print_symbol_table(st);

    buffer_w = malloc(original_size + sizeof(original_size));

    if (buffer_w == NULL)
    {
        printf("Error on allocate output buffer\n");
        return NULL;
    }

    memcpy(buffer_w, &original_size, sizeof(original_size));

    buf_w = buffer_w + sizeof(original_size);

    bits_count = sizeof(*buffer_r) << 3;
    trie_node = trie_root;
    buf_w = buffer_w;

    while (compressed_size > 0)
    {
        if ((trie_node->childs[0] == NULL) && (trie_node->childs[1] == NULL))
        {
            *buf_w = trie_node->c;

            trie_node = trie_root;

            ++buf_w;
        }
        else
        {
            bits_count--;

            trie_node = trie_node->childs[(*buf_r >> bits_count) & 1];
        }

        if (bits_count == 0)
        {
            bits_count = sizeof(*buffer_r) << 3;

            --compressed_size;

            if ((bits_offset > 0) && (compressed_size == 1))
            {
                bits_count = bits_offset + 1;
            }

            ++buf_r;
        }

    }

    return buffer_w;
}

int main(int argc, char *argv[])
{
    FILE *fp;
    size_t n;
    unsigned char *buffer_r, *buffer_w;
    char *file_input, *file_output;

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

    //allocate buffer of same size of file
    buffer_r = malloc(buffer_size + 1);

    //read all file
    n = fread((void *)buffer_r, sizeof(*buffer_r), buffer_size + 1, fp);

    if (!feof(fp))
    {
        printf("Error: there are chars left: bytes read = %zu, buffer size = %zu\n", n, buffer_size);
        return 1;
    }

    fclose(fp);

    buffer_w = function(buffer_r, buffer_size);

    if (buffer_w == NULL)
    {
        printf("Error: output buffer is empty\n");
        return 1;
    }

    fp = fopen(file_output, "wb");
    if (fp == NULL)
    {
        printf("Error: fail to open file '%s'\n", file_output);
    }

    size_t size = *((size_t *)buffer_w);

    buffer_w += sizeof(size);

    //write output with size stored in fist size_t value in buffer
    fwrite(buffer_w, sizeof(*buffer_w), size, fp);

    fclose(fp);

    free(buffer_r);
    free(buffer_w);

    return 0;
}
