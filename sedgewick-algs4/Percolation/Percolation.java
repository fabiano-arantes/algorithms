
public class Percolation {
    private boolean[][] sites;           //sites matrix
    private WeightedQuickUnionUF qf;     //quick find object
    private WeightedQuickUnionUF qfFull; //quick find object for 'full' test
    private int maxIndex;                //max index
    private int maxIndexQf;              //max index in quick find data structure
 
    // create N-by-N grid, with all sites blocked
    public Percolation(int N) {
        if (N <= 0) { 
            throw new IllegalArgumentException("Invalid grid size");
        }
        
        //define limits
        maxIndex = N - 1;
        maxIndexQf = N * N + 1;
        
        //instantiate objects
        sites = new boolean[N][N];
        qf = new WeightedQuickUnionUF(maxIndexQf + 1);
        qfFull = new WeightedQuickUnionUF(maxIndexQf); 
    }
    
    private void checkBounds(int i, int j) {
        if ((i < 1) || (j < 1) || (i > (maxIndex + 1)) || (j > (maxIndex + 1))) { 
            throw new IndexOutOfBoundsException("point (" + i + ", " + j 
                                                    + ") out of bounds");
        }
    }
    
    //calculate quick find index for the given point
    private int qfIndex(int y, int x) {
        return x + y * (maxIndex + 1) + 1;
    }
    
    // open site (row i, column j) if it is not open already
    public void open(int i, int j) {
        //check bounds
        checkBounds(i, j);
        
        //change dimension
        int y = i - 1;
        int x = j - 1;

        //check if it's already open
        if (sites[y][x])
        {
            return;
        }
        
        //get index inside quick find
        int qfp = qf.find(qfIndex(y, x));
        int qfpFull = qfFull.find(qfIndex(y, x));
        
        //open site
        sites[y][x] = true;
        
        //check if point is at first row
        if (y == 0)
        {
            qf.union(0, qfp);
            qfFull.union(0, qfpFull);
        }
        
        if (y == maxIndex)
        {
            qf.union(maxIndexQf, qfp);
        }
 
        //check if upper point is open
        if ((y > 0) && sites[y - 1][x])
        {
            qf.union(qfIndex(y - 1, x), qfp);
            qfFull.union(qfIndex(y - 1, x), qfpFull);
        }
        
        //check if lower point is open
        if ((y < maxIndex) && sites[y + 1][x])
        {
            qf.union(qfIndex(y + 1, x), qfp);
            qfFull.union(qfIndex(y + 1, x), qfpFull);
        }
        
        //check if point at right is open
        if ((x < maxIndex) && sites[y][x + 1])
        {
            qf.union(qfIndex(y, x + 1), qfp);
            qfFull.union(qfIndex(y, x + 1), qfpFull);
        }
        
        //check if point at left is open
        if ((x > 0) && sites[y][x - 1])
        {
            qf.union(qfIndex(y, x - 1), qfp);
            qfFull.union(qfIndex(y, x - 1), qfpFull);
        }
    }
    
    // is site (row i, column j) open?
    public boolean isOpen(int i, int j) {
        //check bounds
        checkBounds(i, j);
        
        return sites[i - 1][j - 1];
    }
    
    // is site (row i, column j) full?
    public boolean isFull(int i, int j) {
        //check bounds
        checkBounds(i, j);
        
        return (sites[i - 1][j - 1] && qfFull.connected(qfIndex(i - 1, j - 1), 0));
    }
 
    // does the system percolate?
    public boolean percolates() {
        /*int rootIndex = qf.find(0);
        
        for (int j = 0; j <= maxIndex; ++j)
        {
            if (sites[maxIndex][j])
            {
                if (qf.connected(rootIndex, qfIndex(maxIndex, j))) {
                    return true;
                }
            }
        }
        return false;*/
        
        return qf.connected(maxIndexQf, 0);
    }

    // test client (optional)
    public static void main(String[] args) {
 
        //Percolation p = new Percolation(3);   
        Percolation p = new Percolation(1);   
        
        //p.open(0, 0);
        //p.open(4, 4);
        p.open(1, 1);
        StdOut.println("Percolation: " + p.percolates());  
        
        /*p.open(2, 1);
        StdOut.println("Percolation: " + p.percolates());        
        p.open(1, 2);
        StdOut.println("Percolation: " + p.percolates());
        p.open(1, 3);
        StdOut.println("Percolation: " + p.percolates());
        p.open(3, 2);
        StdOut.println("Percolation: " + p.percolates());
        p.open(3, 3);
        StdOut.println("Percolation: " + p.percolates());       
        p.open(2, 2);
        StdOut.println("Percolation: " + p.percolates());  */  
    }
}