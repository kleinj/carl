/**
 * @file AbstractRootFinder.h
 * @ingroup rootfinder
 * @author Gereon Kremer <gereon.kremer@cs.rwth-aachen.de>
 */

#pragma once

#include "../../interval/Interval.h"
#include "../RealAlgebraicNumber.h"
#include "../UnivariatePolynomial.h"

namespace carl {
namespace rootfinder {

//#define ROOTFINDER_CACHE

/**
 * Base class for a root finding algorithm for a single univariate polynomial.
 * 
 * This class provides some generic routines and an interface for finding the real roots of a univariate polynomial.
 * Whenever a root is found exactly, the polynomial is reduced, that is the linear factor (x - root) is removed from the polynomial.
 * 
 * The basic usage works like this:
 * <ul>
 *	<li>Create RootFinder with a polynomial and an optional interval</li>
 *	<li>The RootFinder immediately performs some basic checks:
 *		<ul>
 *			<li>If 0 is a root, it is added to the set of roots and the polynomial is reduced accordingly</li>
 *			<li>If the degree of the polynomial is at most two, the roots are obtained using a closed formula.</li>
 *		</ul>
 *	</li>
 *	<li>Call getAllRoots() to obtain all real roots.</li>
 * </ul>
 */
template<typename Number>
class AbstractRootFinder {
protected:
	/**
	 * Original polynomial to solve.
	 * Stored for informational purposes.
	 */
	UnivariatePolynomial<Number> originalPolynomial;
	/**
	 * Polynomial to solve.
	 * This polynomial may be changed during the solving process.
	 */
	UnivariatePolynomial<Number> polynomial;
	/**
	 * Interval to be searched.
	 */
	Interval<Number> interval;
	/**
	 * Roots that have been found.
	 * If the instantiated root finder works in an incremental manner, this list may not contain all roots, if it is called before the root finder has indicated that it is finished.
	 */
	std::list<RealAlgebraicNumber<Number>> roots;
	/**
	 * Flag that indicates if the search has finished.
     */
	bool finished;
#ifdef ROOTFINDER_CACHE
	static std::map<UnivariatePolynomial<Number>, std::list<RealAlgebraicNumber<Number>>> cache;
#endif

public:
	/**
	 * Constructor of an AbstractRootFinder.
	 * This constructor already does some preprocessing on the given polynomial.
	 * At first, the polynomial is tested for a root at zero. If there is such a root, it is added to the root list and removed from the polynomial.
	 * If tryTrivialSolver is true, it will then try to solve the polynomial directly using solveTrivial().
	 * If the interval is unbounded (in either direction), the cauchyBound() method is used to the polynomial to construct a bounded interval.
     * @param polynomial Polynomial to solve.
     * @param interval Interval bounding the range of the roots we are looking for.
	 * @param tryTrivialSolver Flag indicating if the trivialSolve() method should be used.
     */
	AbstractRootFinder(
		const UnivariatePolynomial<Number>& _polynomial,
		const Interval<Number>& _interval = Interval<Number>(),
		bool tryTrivialSolver = true
	);
	
	virtual ~AbstractRootFinder() {
	}
	
	/**
	 * Returns the polynomial that is currently processed.
	 * This might not be the polynomial that was given to the RootFinder as linear factors are removed from the polynomial if an exact root is found.
	 * However, it is a factor of the original polynomial.
	 * @return Current polynomial.
	 */
	const UnivariatePolynomial<Number>& getPolynomial() const {
		return this->polynomial;
	}
	/**
	 * Returns the polynomial that was given to the RootFinder.
     * @return Polynomial given to the RootFinder.
     */
	const UnivariatePolynomial<Number>& getOriginalPolynomial() const {
		return this->originalPolynomial;
	}
	
	/**
	 * Convenience method to obtain all roots at once in a sorted order.
	 * Additional calls will not recompute all roots but only return the cached result.
	 * @returns List of roots.
	 */
	std::list<RealAlgebraicNumber<Number>> getAllRoots();
	
protected:
	/**
	 * Adds a new root to the internal root list.
	 * If the root is in numeric representation, it will eliminate the root from the polynomial unless reducePolynomial is false.
	 * @param root Pointer to new root.
	 * @param reducePolynomial Indicates if the polynomial should be reduced.
	 */
	virtual void addRoot(RealAlgebraicNumber<Number> root, bool reducePolynomial = true);
	
	/**
	 * Adds a new root to the internal root list from an interval.
     * @param interval Isolating interval.
     */
	virtual void addRoot(const Interval<Number>& interval);
	
	/**
	 * Informational method for subclasses specifying the maximum degree of the polynomial that solveTrivial() can handle.
     * @return Maximum degree that solveTrivial() can handle.
     */
	uint solveTrivialMaxDegree() const {
		return 2;
	}
	
	/**
	 * Find roots for polynomials of degree at most two and adds them to roots.
	 * The resulting roots will always be in sorted order.
	 * Assumes that the polynomial is not zero.
	 * @returns true, if polynomial has degree at most two.
	 */
	bool solveTrivial();
	
	/**
	 * Find all roots of the polynomial.
	 * This method must be implemented by each subclass.
	 * All subclasses can safely assume the following about the given polynomial:
	 * - It has no root at zero.
	 * - It has degree at least three.
	 */
	virtual void findRoots() = 0;
	
	/**
	 * Indicates if the root finding process has finished.
	 */
	bool isFinished() const {
		return this->finished;
	}
	
	/**
	 * Set status to finished.
	 * Sorts roots that have been found.
     */
	void setFinished() {
		if (! this->isFinished()) {
			this->finished = true;
			this->roots.sort();
#ifdef ROOTFINDER_CACHE
			this->storeInCache();
#endif
		}
	}
#ifdef ROOTFINDER_CACHE
	bool storeInCache() {
		if (cache.find(this->originalPolynomial) == cache.end()) {
			cache[this->originalPolynomial] = this->roots;
			return true;
		}
		return false;
	}
	bool inCache() {
		return cache.find(this->originalPolynomial) != cache.end();
	}
#endif
};

#ifdef ROOTFINDER_CACHE
template<typename Number>
std::map<UnivariatePolynomial<Number>, std::list<RealAlgebraicNumber<Number>>> AbstractRootFinder<Number>::cache;
#endif

}
}

#include "AbstractRootFinder.tpp"
