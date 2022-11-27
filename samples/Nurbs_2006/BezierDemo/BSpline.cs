using System;

public enum BSplineBasisType {
	Periodical,
	Open
}

public class BSplineCoeffs {

	/// <summary>Количество контрольных точек</summary>
	protected int numPoints;
	/// <summary>Степень сплайна (степень = порядок - 1)</summary>
	protected int degree;
	/// <summary>Узловой вектор</summary>
	protected double[] knots;
	/// <summary>Коэффициенты базисных функций
	/// Обращение: coeffs[номер интервала - degree,
	/// номер базисной функции - номер интервала - degree, степень коэффициента]</summary> 
	protected double[,,] coeffs;

	public BSplineCoeffs(int numPoints,int degree,double[] knots) {
		this.numPoints = numPoints;
		this.degree = degree;
		if(knots.Length != numPoints + degree + 1)
			throw new ArgumentException("Wrong size of knots vector","kn");
		this.knots = (double[])knots.Clone();
		ComputeCoefficients();
	}

	public BSplineCoeffs(BSplineBasisType type,int numPoints,int degree,double tmin,double tmax) {
		this.numPoints = numPoints;
		this.degree = degree;
		knots = new double[numPoints + degree + 1];
		if(type == BSplineBasisType.Open) {
			for(int i = 0;i<=degree;i++)
				knots[i] = tmin;
			for(int i = degree+1;i<numPoints;i++)
				knots[i] = tmin + (tmax - tmin)*(i - degree)/(numPoints - degree);
			for(int i = numPoints;i < numPoints + degree + 1;i++)
				knots[i] = tmax;
		} else if(type == BSplineBasisType.Periodical) {
			for(int i = 0;i<knots.Length;i++) 
				knots[i] = tmin + (i - degree)*(tmax - tmin)/(numPoints - degree);
		} else
			throw new ArgumentException("Unknown basis type","type");
		ComputeCoefficients();
	}

	protected void ComputeCoefficients() {
		coeffs = new double[knots.Length - 2*degree - 1, degree + 1, degree + 1];
		for(int i = 0;i < knots.Length - 2*degree - 1;i++) 
			for(int j = 0;j<=degree;j++)
				for(int k = 0;k<=degree;k++)
					coeffs[i,j,k] = ComputeCoefficient(i + degree, degree, i + j,k);
	}

	protected double ComputeCoefficient(int interval,int n,int i,int k) {
		if(n == 0) // Базисная функция нулевой степени
			return (interval == i)?(1.0):(0);
		else { // n > 0
			double result = 0;
			double d1 = knots[i + n + 1] - knots[i + 1];
			double d0 = knots[i + n] - knots[i];
			if(k == 0) { // Нулевая степень коэффициента
				if(d1 > 0)
					result += ComputeCoefficient(interval,n-1,i+1,0)*knots[i + n + 1]/d1;
				if(d0 > 0)
					result -= ComputeCoefficient(interval,n-1,i,0)*knots[i]/d0;
			} else if(k == n) { // Максимальная степень коэффициента
				if(d0 > 0)
					result += ComputeCoefficient(interval,n-1,i,n-1)/d0;
				if(d1 > 0)
					result -= ComputeCoefficient(interval,n-1,i+1,n-1)/d1;
			} else {
				if(d0 > 0) 
					result += (ComputeCoefficient(interval,n-1,i,k-1) - 
						knots[i]*ComputeCoefficient(interval,n-1,i,k))/d0;
				if(d1 > 0)
					result -= (ComputeCoefficient(interval,n-1,i+1,k-1) - 
						knots[i+n+1]*ComputeCoefficient(interval,n-1,i+1,k))/d1;
			}
			return result;
		}
	}

	public int Order {
		get {
			return degree + 1;
		}
	}

	public int Degree {
		get {
			return degree;
		}
	}

	public int PointCount {
		get {
			return numPoints;
		}
	}

	public double[] Knots {
		get {
			return knots;
		}
	}

	/// <summary>Возвращает заранее вычисленный коэффициент базисной функции</summary>
	/// <param name="interval">Номер интервала - degree</param>
	/// <param name="i">Номер базисной функции</param>
	/// <param name="k">Степень при коэффициенте базисной функции</param>
	/// <returns>Значение коэффициента</returns>
	public double GetCoeff(int interval,int i,int k) {
		if(i < interval || i > interval + degree)
			return 0;
		return coeffs[interval,i - interval,k];
	}

	public double GetBasisValue(int interval,int i,double t) {
		if(i < interval || i > interval + degree)
			return 0;
		double result = coeffs[interval,i - interval,degree];
		for(int k = degree-1;k>=0;k--) 
			result = t*result + coeffs[interval,i - interval,k];
		return result;
	}

	public double GetDerivativeValue(int interval,int i,double t) {
		if(i < interval || i > interval + degree)
			return 0;
		double result = degree*coeffs[interval,i - interval,degree];
		for(int k = degree-1;k>=1;k--)
			result = t*result + k*coeffs[interval,i - interval,k];
		return result;
	}

	public double ParameterMin {
		get {
			return knots[degree];
		}
	}

	public double ParameterMax {
		get {
			return knots[numPoints];
		}
	}
}

public class BSplineBasis : BSplineCoeffs {

	public const int DefaultTesselation = 101;
	/// <summary>Количество точек разбиения параметрической кривой</summary>
	protected int tesselation; 
	/// <summary>Значение базисных функций в точках разбиения. Первый индекс - номер
	/// точки разбиения, второй индекс - номер базисной функции
	/// </summary>
	public double[,] b;
	/// <summary>Значения базисный функций в точках разбиения для вычисления производной</summary>
	public double[,] d;
	
	public BSplineBasis(int tesselation,BSplineBasisType type,int numPoints,int degree,double tmin,double tmax) : 
	  base(type,numPoints,degree,tmin,tmax) {
		SetTesselation(tesselation);	
	}

	public BSplineBasis(int tesselation,int numPoints,int degree,double[] knots) : 
	  base(numPoints,degree,knots) {
		SetTesselation(tesselation);
	}

	public BSplineBasis(BSplineBasisType type,int numPoints,int degree,double tmin,double tmax) :
	  this(DefaultTesselation,type,numPoints,degree,tmin,tmax) {
		// Nothing to do here
	}

	public BSplineBasis(int numPoints,int degree,double[] knots ) :
	  this(DefaultTesselation,numPoints,degree,knots) {
		// Nothing to do here
	}

	protected void SetTesselation(int n) {
		if(n < 2) 
			throw new ArgumentException("Tesselation count should be great or equal to 2","n");
		if(tesselation != n) {
			tesselation = n;
			b = new double [tesselation,numPoints];
			d = new double [tesselation,numPoints];
			int interval = degree;
			for(int i = 0;i<tesselation;i++) {
				double t = knots[degree] + i*(knots[numPoints] - knots[degree])/(tesselation - 1);
				while(knots[interval + 1] < t)
					interval++;
				for(int j = 0;j<numPoints;j++) {
					b[i,j] = GetBasisValue(interval - degree,j,t);
					d[i,j] = GetBasisValue(interval - degree,j,t);
				}
			}
		}
	}

	public int Tesselation {
		get {
			return tesselation;
		}
		set {
			SetTesselation(value);
		}
	}

	public double GetBasisValue(int pointIdx,int functionIdx) 
	{
		return b[pointIdx,functionIdx];
	}

	public double GetDerivativeValue(int pointIdx,int functionIdx) 
	{
		return d[pointIdx,functionIdx];
	}
}

/*public class BSpline : BSplineBasis {


}*/