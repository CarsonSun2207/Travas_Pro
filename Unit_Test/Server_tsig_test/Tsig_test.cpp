#include <gtest/gtest.h>
#include "Trafficsig.h"
#include "Server.h"
#include "Readmsg.h"
#include <cstring> // For strcpy

int test_count=0;
// Mock classes for dependencies
class MockReadmsg : public Readmsg {
public:
    MockReadmsg(Server& server) : Readmsg(server) {
        strcpy(rdmsg, "Entered");  // Initial state for testing
    }
};

class MockServer : public Server {
public:
    void sendtrafsig(bool signal) override {
        // Mock implementation for testing
    }
};

class TrafficsigTest : public ::testing::Test {
protected:
    MockServer server;
    MockReadmsg readmsg;
    Trafficsig trafficSignal;

    TrafficsigTest() : readmsg(server), trafficSignal(readmsg, server) {}
};

TEST_F(TrafficsigTest, TrafficSignalHandler) {
    // Initial state should be false
    EXPECT_FALSE(trafficSignal.getTrafficSignal());

    // Simulate the conditions under which trafficSignalHandler should be called
    trafficSignal.testStaticTrafficSignalHandler();

    // After one call, the signal should be true
    EXPECT_TRUE(trafficSignal.getTrafficSignal());

    // Call the handler again to toggle the signal back
    trafficSignal.testStaticTrafficSignalHandler();

    // After the second call, the signal should be false again
    EXPECT_FALSE(trafficSignal.getTrafficSignal());

    // Simulate changing the state to "Exited"
    strcpy(readmsg.rdmsg, "Exited");
    trafficSignal.testStaticTrafficSignalHandler();

    // Verify the signal state if needed based on "Exited"
    EXPECT_TRUE(trafficSignal.getTrafficSignal()); // Adjust based on actual logic
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

