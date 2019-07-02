/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CMath.h"


namespace Konclude {

	namespace Utilities {



		CMath::CMath() {
		}


		CMath::~CMath() {
		}


		qint64 CMath::log2(qint64 x) {
			if(x<Q_INT64_C(0x100000000)){
				// code from website http://www.ssw.uni-linz.ac.at/Teaching/Lectures/Sem/2002/reports/Priewasser/index.html
				if(x<0x10000){
					if(x<0x100){
						if(x<0x10){
							if(x<0x4){
								if(x<0x2)     
									return 0;
								else             
									return 1;
							} else {
								if(x<0x8)       
									return 2;
								else             
									return 3;
							}
						} else {
							if(x<0x40){
								if(x<0x20)       
									return 4;
								else             
									return 5;
							} else {
								if(x<0x80)       
									return 6;
								else             
									return 7;
							}
						}
					} else {
						if(x<0x1000){
							if(x<0x400){
								if(x<0x200)      
									return 8;
								else             
									return 9;
							} else {
								if(x<0x800)     
									return 10;
								else             
									return 11;
							}
						} else {
							if(x<0x4000){
								if(x<0x2000)     
									return 12;
								else             
									return 13;
							} else {
								if(x<0x8000)     
									return 14;
								else             
									return 15;
							}
						}
					}
				} else {
					if(x<0x1000000){
						if(x<0x100000){
							if(x<0x40000){
								if(x<0x20000)    
									return 16;
								else             
									return 17;
							} else {
								if(x<0x80000)    
									return 18;
								else            
									return 19;
							}
						} else {
							if(x<0x400000){
								if(x<0x200000)   
									return 20;
								else             
									return 21;
							} else {
								if(x<0x800000)   
									return 22;
								else             
									return 23;
							}
						}
					} else {
						if(x<0x10000000){
							if(x<0x4000000){
								if(x<0x2000000)  
									return 24;
								else             
									return 25;
							} else {
								if(x<0x8000000)  
									return 26;
								else             
									return 27;
							}
						} else {
							if(x<0x40000000){
								if(x<0x20000000) 
									return 28;
								else             
									return 29;
							} else {
								if(x<0x80000000) 
									return 30;
								else             
									return 31;
							}
						}
					}
				}
			} else {
				return log2(x>>32)+32;
			}
		}



	}; // end namespace Utilities

}; // end namespace Konclude
