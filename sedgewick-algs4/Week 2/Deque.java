import java.util.Iterator;
import java.lang.NullPointerException;


public class Deque<Item> implements Iterable<Item> {
    private class Node{
        Item item;
        Node next;
        Node prev;
    }
    
    private Node first;
    private Node last;
    private int size;
    
    // construct an empty deque
    public Deque(){
    }
    
    // is the deque empty?
    public boolean isEmpty(){
        return first == null;
    }
    
    // return the number of items on the deque
    public int size(){
        return size;
    }
        
    // add the item to the front
    public void addFirst(Item item){
        if(item == null){
            throw new java.lang.NullPointerException("Item is null");
        }
        
        Node temp = new Node();
        temp.item = item;
        
        if(last == null){
            last = temp;
        }
        else{
            first.prev = temp;
            temp.next = first;
        }
        
        first = temp;
        temp = null;
        
        ++size;
    }

    // add the item to the end
    public void addLast(Item item){
        if(item == null)
        {
            throw new java.lang.NullPointerException("Item is null");
        }
        
        Node temp = new Node();
        temp.item = item;
        
        if(first == null){
            first = temp;
        }
        else{
            temp.prev = last;
            last.next = temp;
        }
        
        last = temp;
        temp = null;
        
        ++size;
    }
    
    // remove and return the item from the front
    public Item removeFirst(){
        if(isEmpty())
        {
            throw new java.util.NoSuchElementException("Deque is empty");
        }
        
        Node ret = first;
        
        if(first.next != null){
            first = first.next;
            first.prev = null;
        }
        --size;
        
        return ret.item;
    }
    
    // remove and return the item from the end
    public Item removeLast(){
        if(isEmpty())
        {
            throw new java.util.NoSuchElementException("Deque is empty");
        }
        
        Node ret = last;
        
        if(last.prev != null){
            last = last.prev;
            last.next = null;
        }
        --size;
        
        return ret.item;
    }
    
    // return an iterator over items in order from front to end
    public Iterator<Item> iterator(){
        return new DequeIterator();
    }
    
     private class DequeIterator implements Iterator<Item>
     {
         private Node current = first;
         public boolean hasNext() { return current != null; }
         public void remove() {
             throw new java.lang.UnsupportedOperationException("Action not supported");
         }
         public Item next()
         {
             if(!hasNext())
             {
                 throw new java.util.NoSuchElementException("End of deque");
             }
             Item item = current.item;
             current = current.next;
             return item;
         }
     }
    
    
// unit testing
    public static void main(String[] args){
    }
}