/* *****************************************************************************
 *  Name:    Fabiano Arantes
 *  NetID:   fa
 *  Precept: P00
 *
 *  Description:  Stats for percolation
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;
import edu.princeton.cs.algs4.Stopwatch;

public class PercolationStats {

    private final double mean; // mean of all trials
    private final double stddev; // standard deviation
    private final double confidenceLo; // confidence interval low
    private final double confidenceHi; // confidence interval high

    // perform trials independent experiments on an n-by-n grid
    public PercolationStats(int n, int trials) {
        if (trials <= 0)
            throw new java.lang.IllegalArgumentException("Illegal trials value: " + n);

        double[] ps = new double[trials];
        int p;
        int row, col;

        for (int t = 0; t < trials; ++t) {
            p = 0;
            Percolation perc = new Percolation(n);

            while (!perc.percolates()) {
                row = StdRandom.uniform(1, n + 1);
                col = StdRandom.uniform(1, n + 1);

                if (!perc.isOpen(row, col)) {
                    ++p;
                    perc.open(row, col);
                }
            }

            ps[t] = p / (double) (n * n);

        }

        this.mean = StdStats.mean(ps);
        this.stddev = StdStats.stddev(ps);

        double confidence = 1.96 * this.stddev / Math.sqrt(trials);

        this.confidenceLo = this.mean - confidence;
        this.confidenceHi = this.mean + confidence;
    }

    // sample mean of percolation threshold
    public double mean() {
        return mean;
    }

    // sample standard deviation of percolation threshold
    public double stddev() {

        return stddev;
    }

    // low  endpoint of 95% confidence interval
    public double confidenceLo() {
        return confidenceLo;
    }

    // high endpoint of 95% confidence interval
    public double confidenceHi() {
        return confidenceHi;
    }

    // test client (described below)
    public static void main(String[] args) {

        Stopwatch sw = new Stopwatch();

        PercolationStats ps = new PercolationStats(Integer.parseInt(args[0]),
                                                   Integer.parseInt(args[1]));

        StdOut.println("mean                    = " + ps.mean());
        StdOut.println("stddev                  = " + ps.stddev());
        StdOut.println("95% confidence interval = [" +
                               ps.confidenceLo() + ", " +
                               ps.confidenceHi() + "]");

        StdOut.println("elapsed time = " + sw.elapsedTime());
    }
}
