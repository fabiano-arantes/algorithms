

public class PercolationStats {
    private double mean;
    private double stdDev;
    private double confHi;
    private double confLo;

    // perform T independent experiments on an N-by-N grid
    public PercolationStats(int N, int T) {
        if ((N <= 0) || (T <= 0)) { 
            throw new IllegalArgumentException("Invalid grid size");
        }
        
        int i, j;
        double[] fractions = new double[T];
        
        int maxNumPoints = N * N;
        int numOpenPoints;
        Percolation p;
        
        for (int t = 0; t < T; ++t)
        {
            p = new Percolation(N);
         
            numOpenPoints = 0;
            while (!p.percolates())
            {
                i = StdRandom.uniform(1, N + 1);
                j = StdRandom.uniform(1, N + 1);
                     
                if (!p.isOpen(i, j))
                {   
                    p.open(i, j);
                    
                    //increment num of open points
                    ++numOpenPoints;
                }
            }
            fractions[t] = (double) numOpenPoints / (double) maxNumPoints;
        }
        
        mean = StdStats.mean(fractions);
        stdDev = StdStats.stddev(fractions);
        
        double term = 1.96 * stdDev / (double) Math.sqrt(T);
        
        confHi = mean + term;
        confLo = mean - term;
        
    }
    
    // sample mean of percolation threshold
    public double mean() {
        return mean;
    }
    
    // sample standard deviation of percolation threshold
    public double stddev() {
        return stdDev;
    }
    
    // low  endpoint of 95% confidence interval
    public double confidenceLo() {
        return confLo;
    }
    
    // high endpoint of 95% confidence interval
    public double confidenceHi() {
        return confHi;
    }
        
    // test client (described below)
    public static void main(String[] args) {
        PercolationStats ps = new PercolationStats(StdIn.readInt(), StdIn.readInt());
        
        StdOut.println("mean                    = " + ps.mean());
        StdOut.println("stddev                  = " + ps.stddev());
        StdOut.println("95% confidence interval = " + ps.confidenceLo() + ", " 
                                                    + ps.confidenceHi());
    }    
}