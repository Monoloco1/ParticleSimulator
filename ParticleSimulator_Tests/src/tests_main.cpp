#include "../../ParticleSimulator_PhysicsLib/include/Physics.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(ParticlePhysics_Tests, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

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
	D offX{};
	D offY{};
	ASSERT_TRUE( testPhysics.collisionDetect(0, 1, offX, offY) );

	testParticle2.setPos( { 1.0, 100.0 } );
	testPhysics.setParticles( 1, testParticle2 );
	ASSERT_FALSE(testPhysics.collisionDetect(0, 1, offX, offY));
}


int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}