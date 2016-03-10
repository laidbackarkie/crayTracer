//Author: Adrion Q Arkenberg
/*
	testing file for pa1 (model/matrix transformations)
	cs410 - CSU
*/
//include necessary assignment files
#include "model.h"
#include "matrix.h"
#include "sphere.h"
#include "point.h"
//get gtest library
#include <gtest/gtest.h>

using namespace std;

/* NOTE: TEST FILE FOR EVERY CLASS FROM NOW ON?? SO SIMILAR TO JAVA */


//open up test writing area for googletests
namespace{
	//----------------------------------------------------------------------
	TEST(matrixMultiply, matMultVertex){
		Matrix* testMat = new Matrix();
		vector<vector<double> > testVert;
		vector<vector<double> > final;
		vector<double> first;
		first.push_back(.557);
		first.push_back(5.1);
		first.push_back(4.2);
		first.push_back(1);
		vector<double> two;
		two.push_back(.557);
		two.push_back(5.1);
		two.push_back(4.2);
		first.push_back(1);
		vector<double> three;
		three.push_back(.557);
		three.push_back(5.1);
		three.push_back(4.2);
		first.push_back(1);
		testMat->elements.push_back(first);
		testMat->elements.push_back(two);
		testMat->elements.push_back(three);
		testVert.push_back(first);

		//final = testMat->multiply(testVert, 1);

		delete testMat;

	}//end martix times vertex test
	//----------------------------------------------------------------------

	TEST(matrixMagnitude, test1){
		Matrix* testMat = new Matrix();
		vector<double> first;
		first.push_back(.557);
		first.push_back(5.1);
		first.push_back(4.2);
		first.push_back(1);

		double mag = testMat->magnitude(first);

		EXPECT_LT(mag, 6.706);
		EXPECT_GT(mag, 6.704);

		//clean up mem
		delete testMat;

	}//END vector magnitude test1
	//----------------------------------------------------------------------

	TEST(matrixMakeUnit, test1){
		Matrix* testMat = new Matrix();
		vector<double> first;
		first.push_back(.557);
		first.push_back(5.1);
		first.push_back(4.2);
		first.push_back(1);

		vector<double> unit;
		testMat->makeUnit(first, unit);

		EXPECT_GT(unit[0], .082);
		EXPECT_LT(unit[0], .084);

		//clean up mem
		delete testMat;

	}//end vector makeUnit test1
	//----------------------------------------------------------------------

	TEST(matrixCrossProduct, test1){
		Matrix* testMat = new Matrix();
		vector<double> first;
		first.push_back(5.5);
		first.push_back(4.5);
		first.push_back(4.25);
		first.push_back(1);
		vector<double> second;
		second.push_back(5.55);
		second.push_back(10);
		second.push_back(3.75);
		second.push_back(1);

		vector<double> xProd;
		testMat->crossProd(first, second, xProd);

		EXPECT_EQ(xProd[0], -25.625);
		EXPECT_GT(xProd[1], 2.9624);
		EXPECT_LT(xProd[1], 2.9626);
		EXPECT_GT(xProd[2], 30.024);
		EXPECT_LT(xProd[2], 30.026);

		//clean up mem
		delete testMat;

	}//end crossProd test1
	//----------------------------------------------------------------------

	TEST(matrixTranspose, test1){
		Matrix* testMat = new Matrix();
		vector<double> first;
		first.push_back(-2/sqrt(12));
		first.push_back(2/sqrt(12));
		first.push_back(-2/sqrt(12));
		first.push_back(0);
		vector<double> second;
		second.push_back(1/sqrt(2));
		second.push_back(0);
		second.push_back(-1/sqrt(2));
		second.push_back(0);
		vector<double> third;
		third.push_back(1/sqrt(6));
		third.push_back(2/sqrt(6));
		third.push_back(1/sqrt(6));
		third.push_back(0);
		vector<double> fourth;
		fourth.push_back(0);
		fourth.push_back(0);
		fourth.push_back(0);
		fourth.push_back(1);
		testMat->elements.push_back(first);
		testMat->elements.push_back(second);
		testMat->elements.push_back(third);
		testMat->elements.push_back(fourth);

		vector<vector<double> > trans = testMat->transpose();

		EXPECT_EQ(trans[0][1], 1/sqrt(2));
		EXPECT_EQ(trans[2][3], 0);
		EXPECT_EQ(trans[1][2], 2/sqrt(6));

		//clean up mem
		delete testMat;

	}//end crossProd test1
	//----------------------------------------------------------------------

	// TEST(vectorSubTest, test1){
	// 	Matrix* tester = new Matrix();
	// 	vector<double> a(4,1.0);
	// 	vector<double> b(4,-4.5);
	// 	vector<double> diff;
	// 	tester->vectorSub(a,b,diff);
	//
	// 	EXPECT_EQ(diff[0], -5.5);
	// 	EXPECT_EQ(diff[0], 5.5);//make sure diff is happening in right order SHOULD FAIL
	// }//end vector subtraction test
	//----------------------------------------------------------------------

	// TEST(vectorAdd, test1){
	// 	Matrix* tester = new Matrix();
	// 	vector<double> a(4,1.0);
	// 	vector<double> b(4,-4.5);
	// 	vector<double> sum;
	// 	tester->vectorAdd(a,b,sum);
	//
	// 	EXPECT_EQ(sum[0], -3.5);
	// }//end vector add test
	//----------------------------------------------------------------------

	TEST(dotProd, test1){
		Matrix* tester = new Matrix();
		vector<double> a(4,1.0);
		vector<double> b(4,-4.25);
		double prod = tester->dotProd(a,b);

		EXPECT_EQ(prod, -12.75);

		//clean up mem
		delete tester;
	}//end dotProd test
	//----------------------------------------------------------------------

	TEST(scalarMult, test1){
		Matrix* tester = new Matrix();
		double a = 4.25;
		vector<double> b(4,-3.25);
		vector<double> sum;
		tester->scalarMult(a,b,sum);

		EXPECT_EQ(sum[0], -13.8125);

		//clean up mem
		delete tester;
	}//end scalarMult test
	//----------------------------------------------------------------------

	TEST(zeroVector, test1){
		Matrix* tester = new Matrix();
		vector<double> a(4,0.0);
		vector<double> b(4,-3.25);
		vector<double> c;
		c.push_back(0);
		c.push_back(0);
		c.push_back(.05);

		EXPECT_TRUE(tester->checkZeroVec(a));
		ASSERT_FALSE(tester->checkZeroVec(b));
		ASSERT_FALSE(tester->checkZeroVec(c));

		//clean up mem
		delete tester;
	}//end 0-vector test
	//----------------------------------------------------------------------

	TEST(centerFinder, test1){
		Matrix* tester = new Matrix();
		vector<Point*> holder;
		vector<double> a;
		a.push_back(1);
		a.push_back(1);
		a.push_back(6);
		vector<double> b;
		b.push_back(3);
		b.push_back(2);
		b.push_back(1);
		vector<double> c;
		c.push_back(2);
		c.push_back(3);
		c.push_back(2);
		//add to point vector
		holder.push_back(new Point(a));
		holder.push_back(new Point(b));
		holder.push_back(new Point(c));

		Sphere* cen = new Sphere(holder);

		EXPECT_EQ(cen->getCenter()->getCoords()[0], 2);
		EXPECT_EQ(cen->getCenter()->getCoords()[1], 2);
		EXPECT_EQ(cen->getCenter()->getCoords()[2], 3);

		//clean up mem
		delete tester;
		for(int i = 0; i < holder.size(); i++){
			delete holder[i];
		}
		delete cen;
	}//end center of points test
	//----------------------------------------------------------------------

	TEST(radiusFinder, test1){
		Matrix* tester = new Matrix();
		vector<Point*> holder;
		vector<double> a;
		a.push_back(1);
		a.push_back(1);
		a.push_back(6);
		vector<double> b;
		b.push_back(3);
		b.push_back(2);
		b.push_back(1);
		vector<double> c;
		c.push_back(2);
		c.push_back(3);
		c.push_back(2);
		//add to point vector
		holder.push_back(new Point(a));
		holder.push_back(new Point(b));
		holder.push_back(new Point(c));

		Sphere* cen = new Sphere(holder);

		double rad = cen->getRadius();

		EXPECT_EQ(rad, sqrt(11));

		//clean up mem
		delete tester;
		for(int i = 0; i < holder.size(); i++){
			delete holder[i];
		}
		delete cen;
	}//end radius of sphere test
	//----------------------------------------------------------------------

}	//END TEST WRITING SPACE

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
