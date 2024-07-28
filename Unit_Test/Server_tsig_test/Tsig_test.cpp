#include <gtest/gtest.h>
#include "Trafficsig.h"
#include "Server.h"
#include "Readmsg.h"
#include <cstring> // For strcpy

int test_count = 0;

// Mock classes for dependencies

// Mock implementation of Readmsg
class MockReadmsg : public Readmsg {
public:
    // Constructor initializes the base class with the server reference
    MockReadmsg(Server& server) : Readmsg(server) {
        strcpy(rdmsg, "Entered");  // Initial state for testing
    }
};

// Mock implementation of Server
class MockServer : public Server {
public:
    // Override the sendtrafsig method for testing
    void sendtrafsig(bool signal) override {
        // Mock implementation for testing
    }
};

// Test fixture for Trafficsig
class TrafficsigTest : public ::testing::Test {
protected:
    MockServer server;            // Mock server instance
    MockReadmsg readmsg;          // Mock readmsg instance
    Trafficsig trafficSignal;     // Trafficsig instance

    // Constructor initializes the readmsg and trafficSignal with the mock server
    TrafficsigTest() : readmsg(server), trafficSignal(readmsg, server) {}
};

// Test case for TrafficSignalHandler
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

// Main function to run all the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); // Run all the tests
}
