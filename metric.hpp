template <UInt dim>
Real metric_product(
    Matrix<dim,dim> m,
    Vector<dim> v) {
  return v * (m * v);
}

template <UInt dim>
Vector<dim> metric_eigenvalues(Vector<dim> h) {
  Vector<dim> l;
  for (UInt i = 0; i < dim; ++i)
    l[i] = 1.0 / square(h[i]);
  return l;
}

template <UInt dim>
Vector<dim> metric_lengths(Vector<dim> l) {
  Vector<dim> h;
  for (UInt i = 0; i < dim; ++i)
    h[i] = 1.0 / sqrt(l[i]);
  return h;
}

template <UInt dim>
Matrix<dim,dim> compose_metric(
    Matrix<dim,dim> r,
    Vector<dim> h) {
  auto l = metric_eigenvalues(h);
  return r * diagonal(l) * transpose(r);
}

template <UInt dim>
void decompose_metric(
    Matrix<dim,dim> m,
    Matrix<dim,dim>& r,
    Vector<dim>& h) {
  Vector<dim> l;
  bool ok = decompose_eigen(m, r, l);
  CHECK(ok);
  h = metric_lengths(l);
}

/* INRIA knows what they're doing with respect to the metric.
   See these papers:

   Frey, Pascal-Jean, and Frédéric Alauzet.
   "Anisotropic mesh adaptation for CFD computations."
   Computer methods in applied mechanics and engineering
   194.48 (2005): 5068-5082.

   F. Alauzet, P.J. Frey, Estimateur d'erreur geometrique
   et metriques anisotropes pour l'adaptation de maillage.
   Partie I: aspects theoriques,
   RR-4759, INRIA Rocquencourt, 2003.

https://www.rocq.inria.fr/gamma/Frederic.Alauzet/
*/

template <UInt dim>
Matrix<dim,dim> common_basis(
    Matrix<dim,dim> a,
    Matrix<dim,dim> b) {
  auto c = invert(a) * b;
  Matrix<dim,dim> p;
  Vector<dim> l;
  bool ok = decompose_eigen(c, p, l);
  CHECK(ok);
  return p;
}

template <UInt dim>
Matrix<dim,dim> intersect_metric(
    Matrix<dim,dim> a,
    Matrix<dim,dim> b) {
  std::cerr << "metric a\n" << a;
  std::cerr << "metric b\n" << b;
  auto p = common_basis(a, b);
  std::cerr << "common basis\n" << p;
  Vector<dim> w;
  for (UInt i = 0; i < dim; ++i) {
    Real u = metric_product(a, p[i]);
    Real v = metric_product(b, p[i]);
    std::cerr << i << ": u " << u << " v " << v << '\n';
    w[i] = max2(u, v);
  }
  auto mi = compose_eigen(p, w);
  std::cerr << "a intersect b\n" << mi;
  return mi;
}

template <UInt dim>
Matrix<dim,dim> interpolate_metric(
    Matrix<dim,dim> a,
    Matrix<dim,dim> b,
    Real t) {
  auto p = common_basis(a, b);
  Vector<dim> w;
  for (UInt i = 0; i < dim; ++i) {
    Real u = metric_product(a, p[i]);
    Real v = metric_product(b, p[i]);
    Real h1 = 1.0 / sqrt(u);
    Real h2 = 1.0 / sqrt(v);
    Real h = ((1.0 - t) * h1) + (t * h2);
    w[i] = 1.0 / square(h);
  }
  return compose_eigen(p, w);
}
