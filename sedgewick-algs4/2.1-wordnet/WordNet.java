import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.ST;
import edu.princeton.cs.algs4.DirectedCycle;
import java.util.LinkedList;  

public class WordNet {

    private ST<String, LinkedList<Integer>> st;
    private ST<Integer, String> nouns;
    //private String[] nouns;
    private Digraph wnet;
    private SAP sap;
    
    // constructor takes the name of the two input files
    public WordNet(String synsetsFile, String hypernymsFile)
    {
        if (synsetsFile == null) 
        {
            throw new java.lang.NullPointerException("Invalid synsetsFile");
        }
        
        if (hypernymsFile == null)
        {
            throw new java.lang.NullPointerException("Ivalid hypernymsFile");
        }
        
        int synsetCount = processSynsets(synsetsFile);
        
        processHypernyms(synsetCount, hypernymsFile);
               
        //detect directed cycle
        DirectedCycle dc = new DirectedCycle(wnet);
        if (dc.hasCycle())
        {
            throw new java.lang.IllegalArgumentException("Invalid WordNet: cycle detected");
        }
        
        
        boolean rootFound = false;
        //check root
        for (int v = 0; v < wnet.V(); ++v)
        {
            if (wnet.outdegree(v) == 0)
            {
                if (rootFound)
                {
                    throw new java.lang.IllegalArgumentException("More than one root found");
                }
                rootFound = true;
            }
        }
        
        //create SAP
        sap = new SAP(wnet);
    }
    
    private int processSynsets(String synsetsFile)
    {
        int synsetCount = 0;
        
        //create symbol table noun -> struct
        st = new ST<String, LinkedList<Integer>>();

        //create array id -> struct
        nouns = new ST<Integer, String>();

        //read synset list
        In inSynsets = new In(synsetsFile);         
        while (inSynsets.hasNextLine())
        {
            ++synsetCount;
            
            //split synset
            String[] a = inSynsets.readLine().split(",");
            
            int id = Integer.parseInt(a[0]);
            
            nouns.put(id, a[1]);
            
            //split nouns
            String[] n = a[1].split(" ");
            
            //fill symbol table noun -> struct
            for (int j = 0; j < n.length; ++j)
            {
                LinkedList<Integer> ids;
                
                if (st.contains(n[j]))
                {
                    ids = st.get(n[j]);
                }    
                else
                {
                    ids = new LinkedList<Integer>();
                }
                
                ids.add(id);
                st.put(n[j], ids);
            }
            
        }
        
        return synsetCount;
    }
        
    private void processHypernyms(int synsetCount, String hypernymsFile)
    {
        //create wordnet
        wnet = new Digraph(synsetCount);
        
        //fill digraph with wordnet edges
        In inHypernyms = new In(hypernymsFile);
        while (inHypernyms.hasNextLine())
        {
            String[] a = inHypernyms.readLine().split(",");
            
            int v = Integer.parseInt(a[0]);
            
            for (int i = 1; i < a.length; ++i)
            {
                wnet.addEdge(v, Integer.parseInt(a[i]));
            }
        }
        
    }
    
    // returns all WordNet nouns
    public Iterable<String> nouns()
    {
        return st.keys();
    }
    
    // is the word a WordNet noun?
    public boolean isNoun(String word)
    {
        if (word == null)
        {
            throw new java.lang.NullPointerException("Ivalid word");
        }
        
        return st.contains(word);
    }
    
    // distance between nounA and nounB (defined below)
    public int distance(String nounA, String nounB)
    {
        if (nounA == null)
        {
            throw new java.lang.NullPointerException("Ivalid nounA");
        }
        
        if (nounB == null)
        {
            throw new java.lang.NullPointerException("Ivalid nounB");
        }
        
        if (!st.contains(nounA))
        {
            throw new java.lang.IllegalArgumentException("nounA is invalid WordNet noun");
        }
           
        if (!st.contains(nounB))
        {
            throw new java.lang.IllegalArgumentException("nounB is invalid WordNet noun");
        }
        
        return sap.length(st.get(nounA), st.get(nounB));
    }
    
    // a synset (second field of synsets.txt) that is the common ancestor of nounA and nounB
    // in a shortest ancestral path (defined below)
    public String sap(String nounA, String nounB)
    {
        if (nounA == null)
        {
            throw new java.lang.NullPointerException("Ivalid nounA");
        }
        
        if (nounB == null)
        {
            throw new java.lang.NullPointerException("Ivalid nounB");
        }
        
        //get ancestor id
        int ancestor = sap.ancestor(st.get(nounA), st.get(nounB));
        
        return nouns.get(ancestor);
    }
    
    // do unit testing of this class
    public static void main(String[] args)
    {
        WordNet wn = new WordNet(args[0], args[1]);
    }
}