import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class SAP {
    private int[] depth;
    private Digraph G;
    
    // constructor takes a digraph (not necessarily a DAG)
    public SAP(Digraph G)
    {
        this.G = G;
    }
    
    private int distTo(int v, int w)
    {
        if (v == w)
        {
            return 0;
        }
        
        Queue<Integer> q = new Queue<Integer>();
        int[] distTo = new int[G.V()];

        q.enqueue(v);
        distTo[v] = 0;
        
        while (!q.isEmpty())
        {
            int a = q.dequeue();
            
            for (int b: G.adj(a))
            {
                distTo[b] = distTo[a] + 1;
                if (b == w)
                {
                    return distTo[b];
                }
                
                q.enqueue(b);
            }
        }
               
        return -1;
    }
    
    // length of shortest ancestral path between v and w; -1 if no such path
    public int length(int v, int w)
    {
        int count;
        int ancestor = ancestor(v, w);
        int distV;
        int distW;
        
        if (ancestor >= 0)
        {
            distV = distTo(v, ancestor);
            distW = distTo(w, ancestor);
            
            return distV + distW;
        }
        
        return -1;
    }
    
    // a common ancestor of v and w that participates in a shortest ancestral path; -1 if no such path
    public int ancestor(int v, int w)
    {
        Queue<Integer> q = new Queue<Integer>();
        boolean[] marked = new boolean[G.V()];
        int ancestor;
        
        q.enqueue(v);
        q.enqueue(w);
        marked[v] = true;
        marked[w] = true;
        
        while (!q.isEmpty())
        {
            int a = q.dequeue();
            for (int b: G.adj(a))
            {
                if (marked[b])
                {
                    return b;
                }
                
                marked[b] = true;
                q.enqueue(b);
            }
        }
        
        return -1;
    }
    
    // length of shortest ancestral path between any vertex in v and any vertex in w; -1 if no such path
    public int length(Iterable<Integer> v, Iterable<Integer> w)
    {
        int shortest = G.V() + 1;
        int len;
        
        for (int iv: v)
        {
            for (int iw: w)
            {
                len = length(iv, iw);
                
                if ((len > 0) && (len < shortest))
                {
                    shortest = len;
                }
            }
        }
        
        if (shortest < (G.V() + 1))
        {
            return shortest;
        }
 
        return -1;
    }
    
    // a common ancestor that participates in shortest ancestral path; -1 if no such path
    public int ancestor(Iterable<Integer> v, Iterable<Integer> w)
    {
        return 0;
    }
    
    // do unit testing of this class
    public static void main(String[] args)
    {
        In in = new In(args[0]);
        Digraph G = new Digraph(in);
        
        StdOut.println(G.toString());
        
        SAP sap = new SAP(G);
        while (!StdIn.isEmpty()) {
            int v = StdIn.readInt();
            int w = StdIn.readInt();
            int length   = sap.length(v, w);
            int ancestor = sap.ancestor(v, w);
            StdOut.printf("length = %d, ancestor = %d\n", length, ancestor);
        }
    }
}