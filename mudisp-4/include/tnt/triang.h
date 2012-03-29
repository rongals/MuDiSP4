// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology


// Triangular Matrices (Views and Adpators)

#ifndef TRIANG_H
#define TRIANG_H

// default to use lower-triangular portions of arrays
// for symmetric matrices.

namespace TNT
{
/** @class LowerTriangularView triang.h <tnt/triang.h>
 * @brief Use lower-triangular portions of arrays for symmetric matrices.
 * @ingroup matrix 
 * */
template <class MaTRiX>
class LowerTriangularView
{
    protected:

		/** @var const MaTRiX  &A_ 
		 * @brief Defines reference to matrix pointer
		 * */ 
        const MaTRiX  &A_;
        /** @var const typename MaTRiX::element_type zero_
		 * @brief Defines  Null elements for Matrix
		 * */ 
        const typename MaTRiX::element_type zero_;

    public:

	/** @typedef typename MaTRiX::const_reference const_reference
	 * @brief define constant reference for class
	 * */
	 
    typedef typename MaTRiX::const_reference const_reference;
    /** @typedef const typename MaTRiX::element_type element_type
     * @brief Defines elements data type for matrix
     * */
    typedef const typename MaTRiX::element_type element_type;
    
    /** @typedef const typename MaTRiX::element_type value_type
     * @brief Defines elements value type
     * */
    typedef const typename MaTRiX::element_type value_type;
    
    /** @typedef element_type T
     * @brief  Defines Template type element for class
     * */
    typedef element_type T;

/** @fn Subscript dim(Subscript d) const 
 * @brief Method for read Matrix Dimension
 * @param d Subscrit limit 
 * @return Dimension of matrix
 * */
    Subscript dim(Subscript d) const {  return A_.dim(d); }
    
  /** @fn Subscript lbound() const
 * @brief Method for read Matrix left Bound
 * @return Index unferior limit
 * */
    Subscript lbound() const { return A_.lbound(); }
    /** @fn Subscript num_rows() const 
     * @brief Method for scan number of rows
     * @return Numbers of rows
     * */
    Subscript num_rows() const { return A_.num_rows(); }
    
    /** @fn Subscript num_cols() const 
     * @brief Method for scan number of columns
     * @return Numbers of columns 
     * */
    Subscript num_cols() const { return A_.num_cols(); }
    
    
    // constructors
/** @fn LowerTriangularView( MaTRiX &A) : A_(A),  zero_(0) 
 * @brief Class Constructors
 * @param A Matrix to be Elaborate
 * */
    LowerTriangularView(/*const*/ MaTRiX &A) : A_(A),  zero_(0) {}

/** @fn   inline const_reference get(Subscript i, Subscript j) const
 * @brief Method for get element of Matrix
 * @param i Row Index
 * @param j Column Index
 * @return return elements \f$ A_{i,j} \f$ 
 * */

    inline const_reference get(Subscript i, Subscript j) const
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(lbound()<=i);
        assert(i<=A_.num_rows() + lbound() - 1);
        assert(lbound()<=j);
        assert(j<=A_.num_cols() + lbound() - 1);
#endif
        if (i<j) 
            return zero_;
        else
            return A_(i,j);
    }

/** @fn  inline const_reference operator() (Subscript i, Subscript j) const
 * @brief Operator for get element of Matrix
 * @param i Row Index
 * @param j Column Index
 * @return return elements \f$ A_{i,j} \f$ 
 * */
    inline const_reference operator() (Subscript i, Subscript j) const
    {
#ifdef TNT_BOUNDS_CHECK
        assert(lbound()<=i);
        assert(i<=A_.num_rows() + lbound() - 1);
        assert(lbound()<=j);
        assert(j<=A_.num_cols() + lbound() - 1);
#endif
        if (i<j) 
            return zero_;
        else
            return A_(i,j);
    }

#ifdef TNT_USE_REGIONS 
  /** @ typedef const_Region2D< LowerTriangularView<MaTRiX> >  const_Region;
   * @brief define a new type for identicate matrix region
   * */
    typedef const_Region2D< LowerTriangularView<MaTRiX> > 
                    const_Region;

	/** @fn  const_Region operator()( Index1D &I, Index1D &J) const
	 * @brief Define operatore for read matrix region
	 * @param I matrix row index pointer reference
	 * @param J Matrix column index pointer reference
	 * @return Matrix region 
	 * */
    const_Region operator()(/*const*/ Index1D &I,
            /*const*/ Index1D &J) const
    {
        return const_Region(*this, I, J);
    }

    /** @fn const_Region operator()(Subscript i1, Subscript i2, Subscript j1, Subscript j2) const
	 * @brief Define operatore for read matrix region
	 * @param i1 matrix row starting index pointer reference
	 * @param i2 Matrix row ending index pointer reference
	 * @param j1 Matrix column starting index  pointer reference
	 * @param j2 Matrix column ending index  pointer reference
	 * @return Matrix region \f$A[i_1-i_2, j_i-j_2]\f$ 
	 * */
    const_Region operator()(Subscript i1, Subscript i2,
            Subscript j1, Subscript j2) const
    {
        return const_Region(*this, i1, i2, j1, j2);
    }



#endif
// TNT_USE_REGIONS

};


/* *********** Lower_triangular_view() algorithms ****************** */
/** @fn template <class MaTRiX, class VecToR> VecToR matmult( LowerTriangularView<MaTRiX> &A, VecToR &x)
 * @brief Moltiplication Algoritm
 * @param A Lower Triangula input Matrix 
 * @param x Moltiplicant Vector
 * @tparam  MaTRiX 	Matrix
 * @tparam  VecToR	Vector
 * @return Cross product \f$A\times x\f$
 * */
template <class MaTRiX, class VecToR>
VecToR matmult(/*const*/ LowerTriangularView<MaTRiX> &A, VecToR &x)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(N == x.dim());

    Subscript i, j;
    typename MaTRiX::element_type sum=0.0;
    VecToR result(M);

    Subscript start = A.lbound();
    Subscript Mend = M + A.lbound() -1 ;

    for (i=start; i<=Mend; i++)
    {
        sum = 0.0;
        for (j=start; j<=i; j++)
            sum = sum + A(i,j)*x(j);
        result(i) = sum;
    }

    return result;
}

/** @fn template <class MaTRiX, class VecToR> inline VecToR operator*( LowerTriangularView<MaTRiX> &A, VecToR &x)
 * @brief Cross Product Operator
 * @param A Lower Triangula input Matrix 
 * @param x Moltiplicant Vector
 * @tparam  MaTRiX 	Matrix
 * @tparam  VecToR	Vector
 * @return Cross product \f$A\times x\f$
 * */
template <class MaTRiX, class VecToR>
inline VecToR operator*(/*const*/ LowerTriangularView<MaTRiX> &A, VecToR &x)
{
    return matmult(A,x);
}

/** @class UnitLowerTriangularView triang.h <tnt triang.h>
 * @brief Define Lower Triangular View Matrix
 * @tparam <class MaTRiX> class
 * @ingroup matrix
 * */
template <class MaTRiX>
class UnitLowerTriangularView
{
    protected:
		/** @var const MaTRiX  &A_
		 * @brief define matrix pointer reference  */
        const MaTRiX  &A_;
        /** @typedef const typename MaTRiX::element_type zero
         * @brief define element zero 
         * */
        const typename MaTRiX::element_type zero;
        /** @typedef const typename MaTRiX::element_type one
         * @brief define element one
         * */
        const typename MaTRiX::element_type one;

    public:

	/** @typedef typename MaTRiX::const_reference const_reference
	 * @brief Defines constant reference type for class */
    typedef typename MaTRiX::const_reference const_reference;
    /** @typedef typename MaTRiX::element_type element_type
	 * @brief Defines constant element type for class */
    typedef typename MaTRiX::element_type element_type;
     /** @typedef typename MaTRiX::element_type value_type
	 * @brief Defines element value type for class */
    typedef typename MaTRiX::element_type value_type;
    
  /** @fn Subscript lbound() const
  * @brief Method for read Matrix left Bound
  * @return 1
  * */
    Subscript lbound() const { return 1; }
   
    /** @typedef element_type T
     * @brief  Defines Template type element for class
     * */
    typedef element_type T;

/** @fn Subscript dim(Subscript d) const 
 * @brief Method for read Matrix Dimension
 * @param d Subscrit limit 
 * @return Dimension of matrix
 * */
    Subscript dim(Subscript d) const {  return A_.dim(d); }
    

    /** @fn Subscript num_rows() const 
     * @brief Method for scan number of rows
     * @return Numbers of rows
     * */
    Subscript num_rows() const { return A_.num_rows(); }
    
    /** @fn Subscript num_cols() const 
     * @brief Method for scan number of columns
     * @return Numbers of columns 
     * */
    Subscript num_cols() const { return A_.num_cols(); }
    
    
    // constructors
  /** @fn UnitLowerTriangularView( MaTRiX &A) : A_(A), zero(0), one(1)
   * @brief Class Constructor
   * @param A Imput Matrix
   * */
    UnitLowerTriangularView(/*const*/ MaTRiX &A) : A_(A), zero(0), one(1) {}


	/** @fn inline const_reference get(Subscript i, Subscript j) const 
	 * @brief Method for get Matrix Elements
	 * @param i Row Index
	 * @param j Column Index
	 * @return \f$A(i,j)\f$ if \f$i>j\f$ else if \f$i=j\f$ return 1 or 0 other else */
    inline const_reference get(Subscript i, Subscript j) const
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=A_.dim(1));
        assert(1<=j);
        assert(j<=A_.dim(2));
        assert(0<=i && i<A_.dim(0) && 0<=j && j<A_.dim(1));
#endif
        if (i>j)
            return A_(i,j);
        else if (i==j)
            return one;
        else 
            return zero;
    }

	/** @fn  inline const_reference operator() (Subscript i, Subscript j) const
	 * @brief Operator for get Matrix Elements
	 * @param i Row Index
	 * @param j Column Index
	 * @return \f$A(i,j)\f$ if \f$i>j\f$ else if \f$i=j\f$ return 1 or 0 other else */
    inline const_reference operator() (Subscript i, Subscript j) const
    {
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=A_.dim(1));
        assert(1<=j);
        assert(j<=A_.dim(2));
#endif
        if (i>j)
            return A_(i,j);
        else if (i==j)
            return one;
        else 
            return zero;
    }


#ifdef TNT_USE_REGIONS 
  // These are the "index-aware" features
  /**   @typedef const_Region2D< UnitLowerTriangularView<MaTRiX> >  const_Region
	 * @brief Define "D "index-aware" features
	 * */
    typedef const_Region2D< UnitLowerTriangularView<MaTRiX> > 
                    const_Region;
	
	/** @fn const_Region operator()( Index1D &I,  Index1D &J) const
	 * @brief Operator return a region of matrix bounded from function parameter
	 * @param I Row region bound for matrix
	 * @param J Columns region bound limit for matrix
	 * @return Matrix region
	 * */
    const_Region operator()(/*const*/ Index1D &I,
            /*const*/ Index1D &J) const
    {
        return const_Region(*this, I, J);
    }


	/** @fn const_Region operator()(Subscript i1, Subscript i2, Subscript j1, Subscript j2) const
	 * @brief Define operatore for read matrix region
	 * @param i1 matrix row starting index pointer reference
	 * @param i2 Matrix row ending index pointer reference
	 * @param j1 Matrix column starting index  pointer reference
	 * @param j2 Matrix column ending index  pointer reference
	 * @return Matrix region \f$A[i_1-i_2, j_i-j_2]\f$ 
	 * */
    const_Region operator()(Subscript i1, Subscript i2,
            Subscript j1, Subscript j2) const
    {
        return const_Region(*this, i1, i2, j1, j2);
    }
#endif
// TNT_USE_REGIONS
};

/** @fn template <class MaTRiX> LowerTriangularView<MaTRiX> Lower_triangular_view( MaTRiX &A)
 * @param A Input Matrix
 * @return Lower Triangular View of \f$ A\f$
 * */
template <class MaTRiX>
LowerTriangularView<MaTRiX> Lower_triangular_view(
    /*const*/ MaTRiX &A)
{
    return LowerTriangularView<MaTRiX>(A);
}


/** @fn template <class MaTRiX> LowerTriangularView<MaTRiX> Unit_lower_triangular_view( MaTRiX &A)
 * @param A Input Matrix
 * @return Unit Lower Triangular View of \f$ A\f$
 * */
template <class MaTRiX>
UnitLowerTriangularView<MaTRiX> Unit_lower_triangular_view(
    /*const*/ MaTRiX &A)
{
    return UnitLowerTriangularView<MaTRiX>(A);
}

/** @fn template <class MaTRiX, class VecToR> inline VecToR matmult( UnitLowerTriangularView<MaTRiX> &A, VecToR &x)
 * @brief Cross Produc function
 * @param A Lower Triangula input Matrix 
 * @param x Moltiplicant Vector
 * @tparam  MaTRiX 	Matrix
 * @tparam  VecToR	Vector
 * @return Cross product \f$A\times x\f$
 * */
template <class MaTRiX, class VecToR>
VecToR matmult(/*const*/ UnitLowerTriangularView<MaTRiX> &A, VecToR &x)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(N == x.dim());

    Subscript i, j;
    typename MaTRiX::element_type sum=0.0;
    VecToR result(M);

    Subscript start = A.lbound();
    Subscript Mend = M + A.lbound() -1 ;

    for (i=start; i<=Mend; i++)
    {
        sum = 0.0;
        for (j=start; j<i; j++)
            sum = sum + A(i,j)*x(j);
        result(i) = sum + x(i);
    }

    return result;
}

/** @fn template <class MaTRiX, class VecToR> inline VecToR operator*( UnitLowerTriangularView<MaTRiX> &A, VecToR &x)
 * @brief Cross Product Operator
 * @param A Lower Triangula input Matrix 
 * @param x Moltiplicant Vector
 * @tparam  MaTRiX 	Matrix
 * @tparam  VecToR	Vector
 * @return Cross product \f$A\times x\f$
 * */
template <class MaTRiX, class VecToR>
inline VecToR operator*(/*const*/ UnitLowerTriangularView<MaTRiX> &A, VecToR &x)
{
    return matmult(A,x);
}


//********************** Algorithms *************************************

/** @fn template <class MaTRiX> std::ostream& operator<<(std::ostream &s, const LowerTriangularView<MaTRiX>&A)
 * @brief Output stream operator
 * @param s standard output
 * @param A   Lower Triangular View Matrix
 * @tparam MaTRiX Matrix Object
 * @return Output steam on standard output 
 * */

template <class MaTRiX>
std::ostream& operator<<(std::ostream &s, const LowerTriangularView<MaTRiX>&A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    s << M << " " << N << endl;

    for (Subscript i=1; i<=M; i++)
    {
        for (Subscript j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}

/** @fn template <class MaTRiX> std::ostream& operator<<(std::ostream &s, const UnitLowerTriangularView<MaTRiX>&A)
 * @brief Output stream operator
 * @param s standard output
 * @param A  Unit  Lower Triangular View Matrix
 * @tparam MaTRiX Matrix Object
 * @return Output steam on standard output 
 * */
template <class MaTRiX>
std::ostream& operator<<(std::ostream &s, 
    const UnitLowerTriangularView<MaTRiX>&A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    s << M << " " << N << endl;

    for (Subscript i=1; i<=M; i++)
    {
        for (Subscript j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}



// ******************* Upper Triangular Section **************************
/** @class  UpperTriangularView triang.h <tnt/triang.h>
 *  @brief Upper Triangular Matrix View
 * @ingroup matrix
 * */
template <class MaTRiX>
class UpperTriangularView
{
    protected:

		/** @var MaTRiX  &A_
		 * @brief define matrix pointer reference  */
        /*const*/ MaTRiX  &A_;
        
        /** @var typename MaTRiX::element_type zero_
         * @brief define zero element type
         * */
        /*const*/ typename MaTRiX::element_type zero_;

    public:

	/** @typedef typename MaTRiX::const_reference const_reference
	 * @brief Defines constant reference type for class */
    typedef typename MaTRiX::const_reference const_reference;
    
    /** @typedef typename MaTRiX::element_type element_type
	 * @brief Defines constant element type for class */
    typedef /*const*/ typename MaTRiX::element_type element_type;
    
    /** @typedef typename MaTRiX::element_type value_type
	 * @brief Defines element value type for class */
    typedef /*const*/ typename MaTRiX::element_type value_type;
    
    /** @typedef element_type T
     * @brief  Defines Template type element for class
     * */
    typedef element_type T;

/** @fn Subscript dim(Subscript d) const 
 * @brief Method for read Matrix Dimension
 * @param d Subscrit limit 
 * @return Dimension of matrix
 * */
    Subscript dim(Subscript d) const {  return A_.dim(d); }
    
 /** @fn Subscript lbound() const
  * @brief Method for read Matrix left Bound
  * @return 1
  * */
    Subscript lbound() const { return A_.lbound(); }

    /** @fn Subscript num_rows() const 
     * @brief Method for scan number of rows
     * @return Numbers of rows
     * */    
    Subscript num_rows() const { return A_.num_rows(); }

    /** @fn Subscript num_cols() const 
     * @brief Method for scan number of columns
     * @return Numbers of columns 
     * */    
    Subscript num_cols() const { return A_.num_cols(); }
    
    
    // constructors

/** @fn UpperTriangularView( MaTRiX &A) : A_(A),  zero_(0)
 * @brief Class Constructor
 * @param A Matrix to be elaborate
 * */
    UpperTriangularView(/*const*/ MaTRiX &A) : A_(A),  zero_(0) {}

	/** @fn inline const_reference get(Subscript i, Subscript j) const 
	 * @brief Method for get Matrix Elements
	 * @param i Row Index
	 * @param j Column Index
	 * @return \f$A(i,j)\f$ if \f$i<j\f$ else return 0 */
    inline const_reference get(Subscript i, Subscript j) const
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(lbound()<=i);
        assert(i<=A_.num_rows() + lbound() - 1);
        assert(lbound()<=j);
        assert(j<=A_.num_cols() + lbound() - 1);
#endif
        if (i>j) 
            return zero_;
        else
            return A_(i,j);
    }


	/** @fn  inline const_reference operator() (Subscript i, Subscript j) const
	 * @brief Operator for get Matrix Elements
	 * @param i Row Index
	 * @param j Column Index
	 * @return \f$A(i,j)\f$ if \f$i<j\f$ else return 0 */
    inline const_reference operator() (Subscript i, Subscript j) const
    {
#ifdef TNT_BOUNDS_CHECK
        assert(lbound()<=i);
        assert(i<=A_.num_rows() + lbound() - 1);
        assert(lbound()<=j);
        assert(j<=A_.num_cols() + lbound() - 1);
#endif
        if (i>j) 
            return zero_;
        else
            return A_(i,j);
    }

#ifdef TNT_USE_REGIONS 
  /**   @typedef const_Region2D< UnitLowerTriangularView<MaTRiX> >  const_Region
	* @brief Define "D "index-aware" features
	* */
    typedef const_Region2D< UpperTriangularView<MaTRiX> > 
                    const_Region;

	/** @fn const_Region operator()( Index1D &I,  Index1D &J) const
	 * @brief Operator return a region of matrix bounded from function parameter
	 * @param I Row region bound for matrix
	 * @param J Columns region bound limit for matrix
	 * @return Matrix region
	 * */
    const_Region operator()(const Index1D &I,
            const Index1D &J) const
    {
        return const_Region(*this, I, J);
    }
	
	/** @fn const_Region operator()(Subscript i1, Subscript i2, Subscript j1, Subscript j2) const
     * @brief Define operatore for read matrix region
	 * @param i1 matrix row starting index pointer reference
	 * @param i2 Matrix row ending index pointer reference
	 * @param j1 Matrix column starting index  pointer reference
	 * @param j2 Matrix column ending index  pointer reference
	 * @return Matrix region \f$A[i_1-i_2, j_1-j_2]\f$ 
	 * */
    const_Region operator()(Subscript i1, Subscript i2,
            Subscript j1, Subscript j2) const
    {
        return const_Region(*this, i1, i2, j1, j2);
    }



#endif
// TNT_USE_REGIONS

};


/* *********** Upper_triangular_view() algorithms ****************** */

/** @fn template <class MaTRiX, class VecToR> inline VecToR matmult( UpperTriangularView<MaTRiX> &A, VecToR &x)
 * @brief Cross Product Function
 * @param A Lower Triangula input Matrix 
 * @param x Moltiplicant Vector
 * @tparam  MaTRiX 	Matrix
 * @tparam  VecToR	Vector
 * @return Cross product \f$A\times x\f$
 * */
template <class MaTRiX, class VecToR>
VecToR matmult(/*const*/ UpperTriangularView<MaTRiX> &A, VecToR &x)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(N == x.dim());

    Subscript i, j;
    typename VecToR::element_type sum=0.0;
    VecToR result(M);

    Subscript start = A.lbound();
    Subscript Mend = M + A.lbound() -1 ;

    for (i=start; i<=Mend; i++)
    {
        sum = 0.0;
        for (j=i; j<=N; j++)
            sum = sum + A(i,j)*x(j);
        result(i) = sum;
    }

    return result;
}

/** @fn template <class MaTRiX, class VecToR> inline VecToR operator*( UpperTriangularView<MaTRiX> &A, VecToR &x)
 * @brief Cross Product Operator
 * @param A Lower Triangula input Matrix 
 * @param x Moltiplicant Vector
 * @tparam  MaTRiX 	Matrix
 * @tparam  VecToR	Vector
 * @return Cross product \f$A\times x\f$
 * */
template <class MaTRiX, class VecToR>
inline VecToR operator*(/*const*/ UpperTriangularView<MaTRiX> &A, VecToR &x)
{
    return matmult(A,x);
}

/** @class UnitUpperTriangularView triang.h <tnt/triang.h>
 * @brief Upper Triangular Matrix View with 1 on major Diagonal
 * @tparam MaTRiX Class Object
 * */
template <class MaTRiX>
class UnitUpperTriangularView
{
    protected:
	   /** @var MaTRiX  &A_
		* @brief define matrix pointer reference  */
        const MaTRiX  &A_;
        
        /** @typedef const typename MaTRiX::element_type zero
         * @brief define element zero 
         * */
        const typename MaTRiX::element_type zero;
        
        /** @typedef const typename MaTRiX::element_type one
         * @brief define element one
         * */
        const typename MaTRiX::element_type one;

    public:
  	/** @typedef typename MaTRiX::const_reference const_reference
	 * @brief Defines constant reference type for class */
    typedef typename MaTRiX::const_reference const_reference;
    
    /** @typedef typename MaTRiX::element_type element_type
	 * @brief Defines constant element type for class */    
    typedef typename MaTRiX::element_type element_type;
    
     /** @typedef typename MaTRiX::element_type value_type
	 * @brief Defines element value type for class */    
    typedef typename MaTRiX::element_type value_type;
    
    /** @typedef element_type T
     * @brief  Defines Template type element for class
     * */    
    typedef element_type T;

  /** @fn Subscript lbound() const
  * @brief Method for read Matrix left Bound
  * @return 1
  * */
    Subscript lbound() const { return 1; }
    
/** @fn Subscript dim(Subscript d) const 
 * @brief Method for read Matrix Dimension
 * @param d Subscrit limit 
 * @return Dimension of matrix
 * */    
    Subscript dim(Subscript d) const {  return A_.dim(d); }
    
    /** @fn Subscript num_rows() const 
     * @brief Method for scan number of rows
     * @return Numbers of rows
     * */    
    Subscript num_rows() const { return A_.num_rows(); }
    
    /** @fn Subscript num_cols() const 
     * @brief Method for scan number of columns
     * @return Numbers of columns 
     * */    
    Subscript num_cols() const { return A_.num_cols(); }

    
    // constructors
    
    /** @fn   UnitUpperTriangularView( MaTRiX &A) : A_(A), zero(0), one(1)
     * @brief Class Constructor
     * @param A
     * */
    UnitUpperTriangularView(/*const*/ MaTRiX &A) : A_(A), zero(0), one(1) {}

	/** @fn inline const_reference get(Subscript i, Subscript j) const 
	 * @brief Method for get Matrix Elements
	 * @param i Row Index
	 * @param j Column Index
	 * @return \f$A(i,j)\f$ if \f$i<j\f$ else if \f$i=j\f$ return 1 or 0 other else */
    inline const_reference get(Subscript i, Subscript j) const
    { 
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=A_.dim(1));
        assert(1<=j);
        assert(j<=A_.dim(2));
        assert(0<=i && i<A_.dim(0) && 0<=j && j<A_.dim(1));
#endif
        if (i<j)
            return A_(i,j);
        else if (i==j)
            return one;
        else 
            return zero;
    }

	/** @fn  inline const_reference operator() (Subscript i, Subscript j) const
	 * @brief Operator for get Matrix Elements
	 * @param i Row Index
	 * @param j Column Index
	 * @return \f$A(i,j)\f$ if \f$i<j\f$ else if \f$i=j\f$ return 1 or 0 other else */

    inline const_reference operator() (Subscript i, Subscript j) const
    {
#ifdef TNT_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=A_.dim(1));
        assert(1<=j);
        assert(j<=A_.dim(2));
#endif
        if (i<j)
            return A_(i,j);
        else if (i==j)
            return one;
        else 
            return zero;
    }


#ifdef TNT_USE_REGIONS 
  // These are the "index-aware" features
  
  /**   @typedef const_Region2D< UnitLowerTriangularView<MaTRiX> >  const_Region
	 * @brief Define "D "index-aware" features
	 * */
    typedef const_Region2D< UnitUpperTriangularView<MaTRiX> > 
                    const_Region;

	/** @fn const_Region operator()( Index1D &I,  Index1D &J) const
	 * @brief Operator return a region of matrix bounded from function parameter
	 * @param I Row region bound for matrix
	 * @param J Columns region bound limit for matrix
	 * @return Matrix region
	 * */
    const_Region operator()(const Index1D &I,
            const Index1D &J) const
    {
        return const_Region(*this, I, J);
    }
	/** @fn const_Region operator()(Subscript i1, Subscript i2, Subscript j1, Subscript j2) const
	 * @brief Define operatore for read matrix region
	 * @param i1 matrix row starting index pointer reference
	 * @param i2 Matrix row ending index pointer reference
	 * @param j1 Matrix column starting index  pointer reference
	 * @param j2 Matrix column ending index  pointer reference
	 * @return Matrix region \f$A[i_1-i_2, j_1-j_2]\f$ 
	 * */
    const_Region operator()(Subscript i1, Subscript i2,
            Subscript j1, Subscript j2) const
    {
        return const_Region(*this, i1, i2, j1, j2);
    }
#endif
// TNT_USE_REGIONS
};

 /** @fn template <class MaTRiX> UpperTriangularView<MaTRiX> Upper_triangular_view (MaTRiX &A)
 * @brief Method for get upper triangular view 
 * @param A Reference to Matrix Pointer
 * @return
 */
template <class MaTRiX>
UpperTriangularView<MaTRiX> Upper_triangular_view(
    /*const*/ MaTRiX &A)
{
    return UpperTriangularView<MaTRiX>(A);
}

/** @fn template <class MaTRiX> UnitUpperTriangularView<MaTRiX> Unit_upper_triangular_view (MaTRiX &A)
 * @brief Method for get Unit upper triangular view 
 * @param A Reference to Matrix Pointer
 * @tparam MaTRiX object class
 * @return
 */
template <class MaTRiX>
UnitUpperTriangularView<MaTRiX> Unit_upper_triangular_view(
    /*const*/ MaTRiX &A)
{
    return UnitUpperTriangularView<MaTRiX>(A);
}

/** @fn template <class MaTRiX, class VecToR> VecToR matmult(UnitUpperTriangularView<MaTRiX> &A, VecToR &x)
 * @brief Matrix cross product function
 * @param A Matrix reference Pointer
 * @param x Vector moltiplicat
 * @tparam MaTRiX object class
 * @return Matrix scalar product \f$ A \times x \f$
*/ 
template <class MaTRiX, class VecToR>
VecToR matmult(/*const*/ UnitUpperTriangularView<MaTRiX> &A, VecToR &x)
{
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    assert(N == x.dim());

    Subscript i, j;
    typename VecToR::element_type sum=0.0;
    VecToR result(M);

    Subscript start = A.lbound();
    Subscript Mend = M + A.lbound() -1 ;

    for (i=start; i<=Mend; i++)
    {
        sum = x(i);
        for (j=i+1; j<=N; j++)
            sum = sum + A(i,j)*x(j);
        result(i) = sum + x(i);
    }

    return result;
}

/** @fn template <class MaTRiX, class VecToR> inline VecToR operator*(UnitUpperTriangularView<MaTRiX> &A, VecToR &x)
 * @brief Matrix cross  product Operator
 * @param A Matrix reference Pointer
 * @param x Vector moltiplicat
 * @tparam MaTRix object
 * @return Matrix scalar product \f$ A \times x \f$
*/ 
template <class MaTRiX, class VecToR>
inline VecToR operator*(/*const*/ UnitUpperTriangularView<MaTRiX> &A, VecToR &x)
{
    return matmult(A,x);
}


//********************** Algorithms *************************************


/** @fn template <class MaTRiX> std::ostream& operator<<(std::ostream &s, UpperTriangularView<MaTRiX>&A)
 * @brief Output to standard output operator
 * @param s Standard output 
 * @param A Upper Triangular View Matrix
 * @tparam MaTRiX Matrix Class Object
 * @return Stream \a A to standard output
        */
        
template <class MaTRiX>
std::ostream& operator<<(std::ostream &s, 
    /*const*/ UpperTriangularView<MaTRiX>&A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    s << M << " " << N << endl;

    for (Subscript i=1; i<=M; i++)
    {
        for (Subscript j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}

/** @fn template <class MaTRiX> std::ostream& operator<<(std::ostream &s, UnitUpperTriangularView<MaTRiX>&A)
 * @brief Output to standard output operator
 * @param s Standard output 
 * @param A Unit Upper Triangular View Matrix
 * @tparam MaTRiX Matrix Class Object
 * @return Stream \a A to standard output
        */
template <class MaTRiX>
std::ostream& operator<<(std::ostream &s, 
        /*const*/ UnitUpperTriangularView<MaTRiX>&A)
{
    Subscript M=A.num_rows();
    Subscript N=A.num_cols();

    s << M << " " << N << endl;

    for (Subscript i=1; i<=M; i++)
    {
        for (Subscript j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}

} // namespace TNT





#endif 
//TRIANG_H

