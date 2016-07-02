template <typename Arr>
DEVICE void set_symm(Arr const& a, Int i, Matrix<3,3> symm) {
  a[i * 6 + 0] = symm[0][0];
  a[i * 6 + 1] = symm[1][1];
  a[i * 6 + 2] = symm[2][2];
  a[i * 6 + 3] = symm[1][0];
  a[i * 6 + 4] = symm[2][1];
  a[i * 6 + 5] = symm[2][0];
}

template <typename Arr>
DEVICE void set_symm(Arr const& a, Int i, Matrix<2,2> symm) {
  a[i * 3 + 0] = symm[0][0];
  a[i * 3 + 1] = symm[1][1];
  a[i * 3 + 2] = symm[1][0];
}

template <typename Arr>
DEVICE Matrix<3,3> get_symm_3(Arr const& a, Int i) {
  Matrix<3,3> symm;
  symm[0][0] = a[i * 6 + 0];
  symm[1][1] = a[i * 6 + 1];
  symm[2][2] = a[i * 6 + 2];
  symm[1][0] = a[i * 6 + 3];
  symm[2][1] = a[i * 6 + 4];
  symm[2][0] = a[i * 6 + 5];
  symm[0][1] = symm[1][0];
  symm[1][2] = symm[2][1];
  symm[0][2] = symm[2][0];
  return symm;
}

template <typename Arr>
DEVICE Matrix<2,2> get_symm_2(Arr const& a, Int i) {
  Matrix<2,2> symm;
  symm[0][0] = a[i * 3 + 0];
  symm[1][1] = a[i * 3 + 1];
  symm[1][0] = a[i * 3 + 2];
  symm[0][1] = symm[1][0];
  return symm;
}

/* working around no support for function template specialization */
template <Int dim, typename Arr>
struct SymmAccess;

template <typename Arr>
struct SymmAccess<2, Arr> {
  DEVICE static Matrix<2,2> get(Arr const& a, Int i) {
    return get_symm_2(a, i);
  }
};

template <typename Arr>
struct SymmAccess<3, Arr> {
  DEVICE static Matrix<3,3> get(Arr const& a, Int i) {
    return get_symm_3(a, i);
  }
};

template <Int dim, typename Arr>
DEVICE Matrix<dim,dim> get_symm(Arr const& a, Int i) {
  return SymmAccess<dim,Arr>::get(a, i);
}

template <Int n, class Arr>
DEVICE void set_vector(Arr const& a, Int i, Vector<n> v) {
  for (Int j = 0; j < n; ++j)
    a[i * n + j] = v[j];
}

template <Int n, class Arr>
DEVICE Vector<n> get_vector(Arr const& a, Int i) {
  Vector<n> v;
  for (Int j = 0; j < n; ++j)
    v[j] = a[i * n + j];
  return v;
}

template <Int neev>
DEVICE Few<LO, neev> gather_verts(LOs const& ev2v, Int e) {
  Few<LO, neev> v;
  for (Int i = 0; i < neev; ++i)
    v[i] = ev2v[e * neev + i];
  return v;
}

template <Int neev>
DEVICE Few<Real, neev>
gather_scalars(Reals const& a, Few<LO, neev> v) {
  Few<Real, neev> x;
  for (Int i = 0; i < neev; ++i)
    x[i] = a[v[i]];
  return x;
}

template <Int neev, Int dim>
DEVICE Few<Vector<dim>, neev>
gather_vectors(Reals const& a, Few<LO, neev> v) {
  Few<Vector<dim>, neev> x;
  for (Int i = 0; i < neev; ++i)
    x[i] = get_vector<dim>(a, v[i]);
  return x;
}

template <Int neev, Int dim>
DEVICE Few<Matrix<dim,dim>, neev>
gather_symms(Reals const& a, Few<LO, neev> v) {
  Few<Matrix<dim,dim>, neev> x;
  for (Int i = 0; i < neev; ++i)
    x[i] = get_symm<dim>(a, v[i]);
  return x;
}

INLINE constexpr Int symm_dofs(Int dim) {
  return ((dim + 1) * dim) / 2;
}

template <Int dim>
Reals repeat_symm(LO n, Matrix<dim,dim> symm);

Reals vectors_2d_to_3d(Reals vecs2);
Reals vectors_3d_to_2d(Reals vecs2);

Reals average_field(Mesh* mesh, Int dim, LOs a2e, Int ncomps, Reals v2x);
