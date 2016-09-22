/* 
 * File:   GroebnerBase.h
 * Author: tobias
 *
 * Created on 8. August 2016, 14:53
 */

#pragma once

#include "../../groebner/groebner.h"

namespace carl {
        


/*
 * A class representing a groebner base for an ideal I.
 * Provides several routines needed in order to compute in the factor ring modulo I.
 */
template<typename Number>
class GroebnerBase {
        
public:
        typedef Term<Number> Monomial;
        
private:        
        typedef MultivariatePolynomial<Number> Polynomial;
        
        std::shared_ptr<Ideal<Polynomial>> mBase;
        
public:
        GroebnerBase() : mBase() {}
        
        template<typename InputIt>
        GroebnerBase(InputIt first, InputIt last) {
                GBProcedure<Polynomial, Buchberger, StdAdding> gbobject;
                while(first != last) {
                        gbobject.addPolynomial(*first);
                        first++;
                }
                gbobject.reduceInput();
                gbobject.calculate();
                mBase = std::make_shared<Ideal<Polynomial>>(gbobject.getIdeal());
        }
        
        Polynomial reduce(const Polynomial& p) const {
                Reductor<Polynomial, Polynomial> r(*mBase, p);
                return r.fullReduce();
        }
        
        inline const std::vector<Polynomial>& get() const { 
                return mBase->getGenerators();
        }
        
        bool hasFiniteMon() const;
        
        std::vector<Monomial> cor() const;
        std::vector<Monomial> mon() const;
        std::vector<Monomial> bor() const;
        
        
        std::set<Variable> gatherVariables() const;
};

}

#include "GroebnerBase.tpp"


