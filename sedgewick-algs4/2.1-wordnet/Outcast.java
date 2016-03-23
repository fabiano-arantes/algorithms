import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class Outcast 
{
   private WordNet wnet;
   
   // constructor takes a WordNet object
   public Outcast(WordNet wordnet)      
   {
       wnet = wordnet;
   }
   
   // given an array of WordNet nouns, return an outcast
   public String outcast(String[] nouns)
   {
       int dmax = -1;
       int d, i, j, k;
       String ret = "";

       for (i = 0; i < nouns.length; ++i)
       {
           d = 0;
           
           for (j = 0; j < nouns.length; ++j)
           {
               if (i == j)
               {
                   continue;
               }
               
               d += wnet.distance(nouns[i], nouns[j]);
           }
           
           if (d > dmax)
           {
               dmax = d;
               ret = nouns[i];
           }
       }
       
       return ret;
   }   
   
   // see test client below
   public static void main(String[] args) 
   {
       WordNet wordnet = new WordNet(args[0], args[1]);
       Outcast outcast = new Outcast(wordnet);
       
       for (int t = 2; t < args.length; t++) 
       {
           In in = new In(args[t]);
           String[] nouns = in.readAllStrings();
           StdOut.println(args[t] + ": " + outcast.outcast(nouns));
       }
}
}