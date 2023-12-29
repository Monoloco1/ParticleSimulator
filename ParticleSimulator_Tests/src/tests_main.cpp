#include "../../ParticleSimulator_PhysicsLib/include/Physics.h"
#include <gtest/gtest.h>


TEST(ParticlePhysics_Tests, add_remove_get_Particles) {
	//Create test Physics object
	Physics testPhysics;

	//Initialize test Particle
	Particle testParticle({ 1.0, 1.0 });

	ASSERT_EQ(testPhysics.getParticles().size(), 0);

	testPhysics.addParticle(testParticle);
	ASSERT_EQ(testPhysics.getParticles().size(), 1);
	
	for (int i{}; i < 100; ++i) 
		testPhysics.addParticle(Particle({ (D)i, (D)i * (D)i }));
	ASSERT_EQ(testPhysics.getParticles().size(), 101);

	testPhysics.removeParticles(10);
	ASSERT_EQ(testPhysics.getParticles().size(), 100);

	testPhysics.removeParticles(20, 10);
	ASSERT_EQ(testPhysics.getParticles().size(), 90);
	ASSERT_EQ(testPhysics.getParticles(15).getPos().x, 15.0);
	ASSERT_EQ(testPhysics.getParticles(15).getPos().y, 225.0 );

	// more tests here
}

TEST(ParticlePhysics_Tests, collsionDetection_test) {
	Physics testPhysics;

	//Initialize test Particles
	Particle testParticle1({ 1.0, 1.0 });
	Particle testParticle2({ 2.0, 2.0 });	//should collide
	testPhysics.addParticle(testParticle1);
	testPhysics.addParticle(testParticle2);
	DP offset{};
	ASSERT_TRUE( testPhysics.collisionDetect(0, 1, offset) );

	testParticle2.setPos( { 1.0, 100.0 } );
	testPhysics.setParticles( 1, testParticle2 );
	ASSERT_FALSE(testPhysics.collisionDetect(0, 1, offset));
}

TEST(ParticlePhysics_Tests, hoverDetection_test) {
	Physics testPhysics;

	//Initialize test Particles
	Particle testParticle1({ 0.0, 0.0 }, { 0.0, 0.0 }, 0.0, prt::Red, {-10.0, 10.0, 10.0, -10.0}, DPV());
	testPhysics.addParticle(testParticle1);
	ASSERT_TRUE(testPhysics.hoverDetect(0, {0.0, 0.0}));
	ASSERT_FALSE(testPhysics.hoverDetect(0, { 50.0, 0.0 }));
}

TEST(ParticlePhysics_Tests, collsionReaction_test) {
	Physics testPhysics;

	//Initialize test Particles
	Particle testParticle1({ 1.0, 1.0 });
	Particle testParticle2({ 2.0, 2.0 });	//should collide
	testPhysics.addParticle(testParticle1);
	testPhysics.addParticle(testParticle2);
	DP offset{};
	if (testPhysics.collisionDetect(0, 1, offset)) {
		testPhysics.collisionReaction(0, 1, offset);
	}
	ASSERT_NE(testPhysics.getParticles(0).getPos().x, testParticle1.getPos().x);
	ASSERT_NE(testPhysics.getParticles(0).getPos().y, testParticle1.getPos().y);
	ASSERT_NE(testPhysics.getParticles(1).getPos().x, testParticle2.getPos().x);
	ASSERT_NE(testPhysics.getParticles(1).getPos().y, testParticle2.getPos().y);
}


int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}