#include <gtest/gtest.h>
#include "signal_bus.h"

TEST(SignalBus, RegisterWriter_Success) {
    SignalBus bus;
    bool result = bus.registerWriter("ev.velocity", "ev_model");
    EXPECT_TRUE(result);
}

TEST(SignalBus, RegisterWriter_DuplicateSameOwner) {
    SignalBus bus;
    bus.registerWriter("ev.velocity", "ev_model");
    bool result = bus.registerWriter("ev.velocity", "ev_model");
    EXPECT_TRUE(result);
}

TEST(SignalBus, RegisterWriter_Conflict) {
    SignalBus bus;
    bus.registerWriter("ev.velocity", "ev_model");
    bool result = bus.registerWriter("ev.velocity", "other_model");
    EXPECT_FALSE(result);
}

TEST(SignalBus, Write_And_Read) {
    SignalBus bus;
    bus.registerWriter("ev.velocity", "ev_model");
    bus.write("ev.velocity", 42.0, "ev_model");
    EXPECT_DOUBLE_EQ(bus.read("ev.velocity"), 42.0);
}

TEST(SignalBus, Write_WrongOwner) {
    SignalBus bus;
    bus.registerWriter("ev.velocity", "ev_model");
    bus.write("ev.velocity", 42.0, "ev_model");
    bus.write("ev.velocity", 99.0, "other_model");
    EXPECT_DOUBLE_EQ(bus.read("ev.velocity"), 42.0);
}

TEST(SignalBus, Write_UnregisteredSignal) {
    SignalBus bus;
    bus.write("ev.velocity", 42.0, "ev_model");
    EXPECT_FALSE(bus.has("ev.velocity"));
}

TEST(SignalBus, Read_UnregisteredSignal) {
    SignalBus bus;
    EXPECT_DOUBLE_EQ(bus.read("ev.velocity"), 0.0);
}

TEST(SignalBus, Has_RegisteredSignal) {
    SignalBus bus;
    bus.registerWriter("ev.velocity", "ev_model");
    EXPECT_TRUE(bus.has("ev.velocity"));
}

TEST(SignalBus, Has_UnregisteredSignal) {
    SignalBus bus;
    EXPECT_FALSE(bus.has("ev.velocity"));
}

TEST(SignalBus, InitializeSignal_SetsDefaultValue) {
    SignalBus bus;
    bus.initializeSignal("ev.velocity", 5.0, "ev_model");
    EXPECT_DOUBLE_EQ(bus.read("ev.velocity"), 5.0);
    EXPECT_TRUE(bus.has("ev.velocity"));
}