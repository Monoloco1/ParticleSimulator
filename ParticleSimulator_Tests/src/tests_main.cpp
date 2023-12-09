#include "../../ParticleSimulator_PhysicsLib/include/Physics.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(ParticlePhysics_Tests, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(ParticlePhysics_TestFixture, add_remove_get_Particles) {
	//Create test Physics object
	Physics testPhysics;

	//Initialize test Particle
	Particle testParticle({ 1.0, 1.0 });

	ASSERT_EQ(testPhysics.getParticles().size(), 0);

	testPhysics.addParticle(testParticle);
	ASSERT_EQ(testPhysics.getParticles().size(), 1);
	
	for (int i{}; i < 100; ++i) {
		//const DP dp{};
		//dp.x = (D)i;
		//dp.x = (D)i*i;
		//testParticle.setPos( { (D)i, (D)i * i });
		//testPhysics.addParticle( Particle( const {1.0, 1.0} ));
	}
	ASSERT_EQ(testPhysics.getParticles().size(), 101);

	testPhysics.removeParticles(10);
	ASSERT_EQ(testPhysics.getParticles().size(), 100);

	testPhysics.removeParticles(20, 10);
	ASSERT_EQ(testPhysics.getParticles().size(), 90);


	//std::cout << testPhysics.getParticles(15).getPos().x << " " << testPhysics.getParticles(15).getPos().y;
	//ASSERT_EQ( testPhysics.getParticles(15).getPos(), (DP){15.0, 225.0} );
	/*void removeParticles(int index);
	void removeParticles(int startIndex, int length);
	void addParticle(Particle & p);
	PV getParticles();
	Particle getParticles(int index);*/
}


int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}