/**
 * @file MonomialOrdering.h
 * @ingroup multirp
 */
#pragma once

#include "CompareResult.h"
#include "Monomial.h"
#include "Term.h"

namespace carl 
{

typedef CompareResult( *MonomialOrderingFunction )(const Monomial::Arg&, const Monomial::Arg&);

/**
 * A class for term orderings.
 * @ingroup multirp
 */
template<MonomialOrderingFunction f, bool degreeOrdered>
struct MonomialComparator
{
	static CompareResult compare(const Monomial::Arg& m1, const Monomial::Arg& m2) {
		return f(m1, m2);
	}
    
    template<typename Coeff>
    static CompareResult compare(const Term<Coeff>& t1, const Term<Coeff>& t2) {
        return compare(t1.monomial(), t2.monomial());
    }
    
    template<typename Coeff>
    static bool less(const Term<Coeff>& t1, const Term<Coeff>& t2) {
        return (compare(t1, t2) == CompareResult::LESS );
    }
	
	static bool less(const Monomial::Arg& m1, const Monomial::Arg& m2) {
        return (compare(m1, m2) == CompareResult::LESS );
    }
	
	template<typename Coeff>
    static bool equal(const Term<Coeff>& t1, const Term<Coeff>& t2) {
        return (compare(t1, t2) == CompareResult::EQUAL );
    }
	
	static bool equal(const Monomial::Arg& m1, const Monomial::Arg& m2) {
        return (compare(m1, m2) == CompareResult::EQUAL );
    }

    static const bool degreeOrder = degreeOrdered;
};


typedef MonomialComparator<Monomial::compareLexical, false > LexOrdering;
typedef MonomialComparator<Monomial::compareGradedLexical, true > GrLexOrdering;
//typedef MonomialComparator<Monomial::GrRevLexCompare, true > GrRevLexOrdering;
}