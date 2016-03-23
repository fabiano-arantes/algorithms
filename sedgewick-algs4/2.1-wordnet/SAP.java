import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.BreadthFirstDirectedPaths;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import java.util.LinkedList;  
import java.util.Arrays;    

public class SAP {
    private Digraph G;
    
    private class ShortestPath
    {
        private int ancestor;
        private int length;
    }
    
    // constructor takes a digraph (not necessarily a DAG)
    public SAP(Digraph G)
    {
        if (G == null)
        {
            throw new java.lang.NullPointerException("Ivalid digraph");
        }
        
        this.G = new Digraph(G);
    }
    
    private void checkRange(int v) 
    {
        if (!(v >= 0 && v <= this.G.V() - 1))
        {
            throw new java.lang.IndexOutOfBoundsException("Parameter out of bounds");
        }   
    }
    
    private void isValid(Iterable<Integer> v) 
    {
        if (v == null)
        {
            throw new java.lang.NullPointerException("Null parameter");
        }
        
        for (int integer : v) 
        {
             checkRange(integer);
        }
    }

    private ShortestPath findShortestPath(Iterable<Integer> v, Iterable<Integer> w)
    {
        isValid(v);
        isValid(w);
        
        BreadthFirstDirectedPaths bfsv = new BreadthFirstDirectedPaths(G, v);
        BreadthFirstDirectedPaths bfsw = new BreadthFirstDirectedPaths(G, w);
        int shortestPath = Integer.MAX_VALUE;
        int shortestAncestor = -1;
        
        LinkedList<Integer> ancestors = new LinkedList<Integer>();
        
        for (int i = 0; i < G.V(); ++i)
        {
            if (bfsv.hasPathTo(i) && (bfsw.hasPathTo(i)))
            {
                ancestors.add(i);
            }
        }
        
        for (int p: ancestors)
        {
            int path = bfsv.distTo(p) + bfsw.distTo(p);
            
            if (path < shortestPath)
            {
                shortestPath = path;
                shortestAncestor = p;
            }
        }
        
        ShortestPath sp = new ShortestPath();
 
        sp.ancestor = shortestAncestor;
        if (shortestPath != Integer.MAX_VALUE)
        {
            sp.length = shortestPath;        
        }
        else
        {
            sp.length = -1;
        }
        
        return sp;
    }
    
    // length of shortest ancestral path between v and w; -1 if no such path
    public int length(int v, int w)
    { 
        ShortestPath sp = findShortestPath(Arrays.asList(v), Arrays.asList(w));
                
        return sp.length;
    }
    
    // a common ancestor of v and w that participates in a shortest ancestral path; -1 if no such path
    public int ancestor(int v, int w)
    {  
        ShortestPath sp = findShortestPath(Arrays.asList(v), Arrays.asList(w));
                
        return sp.ancestor;
    }
    
    // length of shortest ancestral path between any vertex in v and any vertex in w; -1 if no such path
    public int length(Iterable<Integer> v, Iterable<Integer> w)
    {
        return findShortestPath(v, w).length;
    }
    
    // a common ancestor that participates in shortest ancestral path; -1 if no such path
    public int ancestor(Iterable<Integer> v, Iterable<Integer> w)
    {
        return findShortestPath(v, w).ancestor;
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