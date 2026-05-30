#include <gtest/gtest.h>
#include "signal_bus.h"
#include "bus_connector.h"

TEST(BusConnector, RegisterWrite_TranslatesCorrectly) {
    SignalBus bus;
    BusConnector connector(&bus, "ev_model");
    connector.registerWrite("velocity", "ev.velocity");
    connector.write("velocity", 17.5);
    EXPECT_DOUBLE_EQ(bus.read("ev.velocity"), 17.5);
}

TEST(BusConnector, RegisterRead_TranslatesCorrectly) {
    SignalBus bus;
    bus.initializeSignal("sim.throttle", 0.7, "default");
    BusConnector connector(&bus, "ev_model");
    connector.registerRead("throttle", "sim.throttle");
    EXPECT_DOUBLE_EQ(connector.read("throttle"), 0.7);
}

TEST(BusConnector, Write_UnregisteredInternalName) {
    SignalBus bus;
    BusConnector connector(&bus, "ev_model");
    connector.write("velocity", 17.5);
    EXPECT_FALSE(bus.has("ev.velocity"));
}

TEST(BusConnector, Read_UnregisteredInternalName) {
    SignalBus bus;
    BusConnector connector(&bus, "ev_model");
    EXPECT_DOUBLE_EQ(connector.read("throttle"), 0.0);
}

TEST(BusConnector, Write_OwnershipEnforced) {
    SignalBus bus;
    BusConnector connector_a(&bus, "ev_model");
    BusConnector connector_b(&bus, "other_model");
    connector_a.registerWrite("velocity", "ev.velocity");
    connector_b.registerWrite("velocity", "ev.velocity");
    connector_a.write("velocity", 10.0);
    connector_b.write("velocity", 99.0);
    EXPECT_DOUBLE_EQ(bus.read("ev.velocity"), 10.0);
}

TEST(BusConnector, MultipleSignals_IndependentMappings) {
    SignalBus bus;
    BusConnector connector(&bus, "ev_model");
    connector.registerWrite("velocity", "ev.velocity");
    connector.registerWrite("acceleration", "ev.acceleration");
    connector.write("velocity", 15.0);
    connector.write("acceleration", 2.5);
    EXPECT_DOUBLE_EQ(bus.read("ev.velocity"), 15.0);
    EXPECT_DOUBLE_EQ(bus.read("ev.acceleration"), 2.5);
}