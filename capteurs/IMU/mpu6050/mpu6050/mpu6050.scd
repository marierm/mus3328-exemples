
~bus = Bus.control(s, 7);
(
OSCdef(\mpu, {
    arg msg;
    var data;
    data = msg[1..];
    ~bus.set(* data);  // * data
    // ~bus.set( * [1,2,3,5,6,7] ); est synonyme de : ~bus.set( 1,2,3,4,5,6,7 );
    //
}, "/mpu6050", recvPort: 55222);
)

BusPlot(~bus, "mpu6050", 3, 20);