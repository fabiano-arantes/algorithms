import java.util.Iterator;
import java.lang.NullPointerException;
    
public class RandomizedDeque<Item> implements Iterable<Item> {    
    private class Node{
        Item item;
        Node next;
        Node prev;
    }
    
    private Node first;
    private Node current;
    private Node last;
    
    // construct an empty randomized deque
    public RandomizedDeque(){
    }
    
    // is the deque empty?
    public boolean isEmpty(){
        return true;
    }
    
    // return the number of items on the deque
    public int size(){
        return 0;
    }
        
    
    
    
    // add the item
    public void enqueue(Item item){
        if(item == null)
        {
            throw new java.lang.NullPointerException("Item is null");
        }
    }
    
    // remove and return the random item
    public Item dequeue(){
        if(isEmpty())
        {
            throw new java.util.NoSuchElementException("RandomizedDeque is empty");
        }
        return null;
    }
    
    // return (but do not remove) a random item
    public Item sample()
    {
        return null;
    }
    
    // return an iterator over items in order from front to end
    public Iterator<Item> iterator(){
        return new RandomizedDequeIterator();
    }
    
     private class RandomizedDequeIterator implements Iterator<Item>
     {
         private Node current = first;
         public boolean hasNext() { return current != null; }
         public void remove() {
             throw new java.lang.UnsupportedOperationException("Action not supported");
         }
         public Item next()
         {
             if(hasNext())
             {
                 throw new java.util.NoSuchElementException("End of RandomizedDeque");
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