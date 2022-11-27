using System;
using System.Collections;
using System.Windows.Forms;

public class BinomialCoeffs {

	protected int[][] c;

	public BinomialCoeffs(int n) {
		c = new int [n+1][];
		for(int i = 0;i<=n;i++) {
			c[i] = new int[i+1];
			c[i][0] = c[i][i] = 1;
			for(int j = 1;j<i;j++)
				c[i][j] = c[i-1][j] + c[i-1][j-1];
		}
	}

	public int this[int n,int k] {
		get {
			return c[n][k];
		}
	}
}

public class BezierCurve {

	protected int n;
	protected Vector2d[] points;
	protected BinomialCoeffs coeffs;

	public BezierCurve(ArrayList pts) {
		n = pts.Count - 1;
		coeffs = new BinomialCoeffs(n);
		points = (Vector2d[])pts.ToArray(typeof(Vector2d));
	}

	public Vector2d[] Points {
		get {
			return points;
		}
	}

	public Vector2d GetPoint(double t) {
		Vector2d result = points[0] * Math.Pow(1-t,n);
		for(int i = 1;i<n;i++) 
			result += points[i] * coeffs[n,i] * Math.Pow(t,i) * Math.Pow(1-t,n-i);
		result += points[n] * Math.Pow(t,n);
		return result;
	}
}
