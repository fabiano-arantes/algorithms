/* *****************************************************************************
 *  Name:    Fabiano Arantes
 *  NetID:   fabiano.arantes
 *  Precept: P00
 *
 *  Description:  Percolation
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    private final int topSitePosition; // position of top site
    private final int bottomSitePosition; // position of bottom site

    private final int n; // grid size
    private int openSitesCount; // count open sites
    private boolean[] openSites; // open sites flags
    private final WeightedQuickUnionUF ufCheckPercolation; // union-find object
    private final WeightedQuickUnionUF ufCheckIsFull; // uf object wit bottom site

    // create n-by-n grid, with all sites blocked
    public Percolation(int n) {

        if (n <= 0)
            throw new java.lang.IllegalArgumentException("Illegal n value: " + n);

        this.n = n;
        this.topSitePosition = n * n;
        this.bottomSitePosition = topSitePosition + 1;
        this.openSitesCount = 0;
        this.openSites = new boolean[n * n + 2];
        this.ufCheckPercolation = new WeightedQuickUnionUF(n * n + 2);
        this.ufCheckIsFull = new WeightedQuickUnionUF(n * n + 1);

        this.openSites[topSitePosition] = true;
        this.openSites[bottomSitePosition] = true;

    }

    // validate row and col values
    private void validate(int row, int col) {

        if ((row < 1) || (row > this.n))
            throw new java.lang.IllegalArgumentException("Illegal row value: " + row);

        if ((col < 1) || (col > this.n))
            throw new java.lang.IllegalArgumentException("Illegal col value: " + col);

    }

    // calculates uf array position
    private int getP(int row, int col) {
        return (row - 1) * this.n + (col - 1);
    }

    // makes unions
    private void union(int p, int q) {
        if (!openSites[p] || !openSites[q])
            return;

        ufCheckPercolation.union(p, q);

        if ((p != bottomSitePosition) && (q != bottomSitePosition))
            ufCheckIsFull.union(p, q);
    }

    // open site (row, col) if it is not open already
    public void open(int row, int col) {

        validate(row, col);

        // get row-col position in uf array
        int p = getP(row, col);

        // checks if is already open
        if (openSites[p])
            return;

        // open new site
        openSites[p] = true;
        ++openSitesCount;

        // check top
        if (row == 1)
            union(topSitePosition, p);
        else
            union(p, getP(row - 1, col));

        if (row == n)
            union(p, bottomSitePosition);
        else
            union(p, getP(row + 1, col));

        // check left neighbor
        if (col > 1)
            union(p, getP(row, col - 1));

        // check right neighbor
        if (col < n)
            union(p, getP(row, col + 1));
    }

    // is site (row, col) open?
    public boolean isOpen(int row, int col) {
        validate(row, col);
        return openSites[getP(row, col)];
    }

    // is site (row, col) full?
    public boolean isFull(int row, int col) {
        validate(row, col);
        return ufCheckIsFull.connected(topSitePosition, getP(row, col));
    }

    // number of open sites
    public int numberOfOpenSites() {
        return openSitesCount;
    }

    // does the system percolate?
    public boolean percolates() {
        return ufCheckPercolation.connected(topSitePosition, bottomSitePosition);
    }

    // test client (optional)
    public static void main(String[] args) {

        boolean exceptionHappened = false;
        final int n = 10;
        Percolation p;

        // ------------- check negative n --------------------

        try {
            p = new Percolation(-1);
            p.percolates();
        }
        catch (java.lang.IllegalArgumentException e) {
            exceptionHappened = true;
        }

        assert (exceptionHappened);

        // ---------------------------------------------------

        exceptionHappened = false;
        p = new Percolation(n);

        // --------- check row and col lesser than 0 ---------

        try {
            p.open(-1, -1);
        }
        catch (java.lang.IllegalArgumentException e) {
            exceptionHappened = true;
        }

        assert (exceptionHappened);
        exceptionHappened = false;

        try {
            p.isOpen(-1, -1);
        }
        catch (java.lang.IllegalArgumentException e) {
            exceptionHappened = true;
        }

        assert (exceptionHappened);

        // ---------------------------------------------------

        // --------- check row and col bigger than n ---------

        try {
            p.open(n + 1, n + 1);
        }
        catch (java.lang.IllegalArgumentException e) {
            exceptionHappened = true;
        }

        assert (exceptionHappened);
        exceptionHappened = false;

        try {
            p.isOpen(n + 1, n + 1);
        }
        catch (java.lang.IllegalArgumentException e) {
            exceptionHappened = true;
        }

        assert (exceptionHappened);

        // ---------------------------------------------------


        // --------------- must not percolate ----------------

        for (int ii = 1; ii < n; ++ii) {
            p.open(ii, 1);
        }

        assert (!p.percolates()) : "It should not percolate!";

        // ---------------------------------------------------

        // --------------- must percolate ----------------

        p.open(10, 1);
        assert (p.percolates()) : "It should percolate!";

        // ---------------------------------------------------

        // --------------- must not be full ------------------

        p.open(10, 3);
        assert (!p.isFull(10, 3)) : "It shouldn't be full!";

        // ---------------------------------------------------

        // ----------------- must be full --------------------

        p.open(10, 2);
        assert (p.isFull(10, 3)) : "It should be full!";

        // ---------------------------------------------------

        // ----------------- check open sites ----------------

        assert (p.numberOfOpenSites() == 12) : "It should be 12!";

        // ---------------------------------------------------

    }
}
