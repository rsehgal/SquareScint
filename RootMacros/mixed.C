#include <TCanvas.h>
#include <TGraph.h>
#include <TF1.h>
#include <vector>

struct Segments {
    double a0, a1, a2, a3; // poly3 coefficients for first segment
    double m, c;           // slope, intercept for middle segment
    double b0, b1, b2, b3; // poly3 coefficients for last segment
    double x1, x2;         // boundaries
} seg;

Double_t mixedFunc(Double_t *x, Double_t *) {
    double xx = x[0];
    if (xx < seg.x1) {
        return seg.a0 + seg.a1*xx + seg.a2*xx*xx + seg.a3*xx*xx*xx;
    }
    else if (xx < seg.x2) {
        return seg.m*xx + seg.c;
    }
    else {
        return seg.b0 + seg.b1*xx + seg.b2*xx*xx + seg.b3*xx*xx*xx;
    }
}

// Helper: solve cubic through two points with zero slope at boundaries (example condition)
void solvePoly3(double x0, double y0, double x1, double y1,
                double &p0, double &p1, double &p2, double &p3)
{
    // Simple interpolation: assume cubic with zero 1st derivative at both ends
    // y = p0 + p1*x + p2*x^2 + p3*x^3
    // Conditions: y(x0)=y0, y(x1)=y1, y'(x0)=0, y'(x1)=0
    double A[4][4] = {
        {1, x0, x0*x0, x0*x0*x0},
        {1, x1, x1*x1, x1*x1*x1},
        {0, 1, 2*x0, 3*x0*x0},
        {0, 1, 2*x1, 3*x1*x1}
    };
    double B[4] = {y0, y1, 0.0, 0.0};

    // Solve linear system (Gaussian elimination)
    for (int i = 0; i < 4; ++i) {
        // pivot
        double maxVal = fabs(A[i][i]);
        int maxRow = i;
        for (int k = i+1; k < 4; ++k) {
            if (fabs(A[k][i]) > maxVal) {
                maxVal = fabs(A[k][i]);
                maxRow = k;
            }
        }
        for (int k = i; k < 4; ++k) std::swap(A[maxRow][k], A[i][k]);
        std::swap(B[maxRow], B[i]);

        // normalize row
        double div = A[i][i];
        for (int k = i; k < 4; ++k) A[i][k] /= div;
        B[i] /= div;

        // eliminate
        for (int r = 0; r < 4; ++r) {
            if (r != i) {
                double factor = A[r][i];
                for (int k = i; k < 4; ++k) A[r][k] -= factor * A[i][k];
                B[r] -= factor * B[i];
            }
        }
    }

    p0 = B[0]; p1 = B[1]; p2 = B[2]; p3 = B[3];
}

void draw_mixed_fit() {
    std::vector<double> xvec = {-2686., -2080., 1564., 2285.};
    std::vector<double> yvec = { -20., -10., 10., 20.};

    // Segment boundaries
    seg.x1 = xvec[1];
    seg.x2 = xvec[2];

    // First cubic
    solvePoly3(xvec[0], yvec[0], xvec[1], yvec[1], seg.a0, seg.a1, seg.a2, seg.a3);

    // Middle linear
    seg.m = (yvec[2] - yvec[1]) / (xvec[2] - xvec[1]);
    seg.c = yvec[1] - seg.m * xvec[1];

    // Last cubic
    solvePoly3(xvec[2], yvec[2], xvec[3], yvec[3], seg.b0, seg.b1, seg.b2, seg.b3);

    // Graph
    TGraph *g = new TGraph(xvec.size());
    for (size_t i = 0; i < xvec.size(); i++) {
        g->SetPoint(i, xvec[i], yvec[i]);
    }
    g->SetMarkerStyle(20);
    g->SetMarkerSize(1.2);

    TF1 *f = new TF1("mixed", mixedFunc, xvec[0], xvec[3], 0);

    TCanvas *c = new TCanvas("c", "Mixed poly fit", 800, 600);
    g->Draw("AP");
    f->SetLineColor(kRed);
    f->Draw("same");

    TFile *cf =new TFile("mixed.root","RECREATE");
    f->Write();
    cf->Close();
}

