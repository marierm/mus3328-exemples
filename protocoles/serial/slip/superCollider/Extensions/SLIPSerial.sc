SLIPSerial : SerialPort {
	var <inputThread, <>action;
	var slipEND = 8r300;
	var slipESC = 8r333;
	var slipESC_END = 8r334;
	var slipESC_ESC = 8r335;


	readSLIP { |func, maxSize=128|
		var packet;
		var byte;
		var packetSize = 0;

		action = func;
		
		packet = Int16Array.newClear(maxSize);
		inputThread = fork {
			// SLIP Decoding
			// =============
			// 
			// SLIP packets are encoded using these special characters:
			// end = 8r300 (2r11000000 or 0xc0 or 192)
			// esc = 8r333 (2r11011011 or 0xdb or 219)
			// esc_end = 8r334 (2r011011100 or 0xdc or 220)
			// esc_esc = 8r335 (2r011011101 or 0xdd or 221)
			{

				byte = this.read;
				byte.switch(
					slipEND, {
						(packetSize > 0).if({
							action.value(packet.keep(packetSize));
						});
						packetSize = 0;
					},
					slipESC, {
						byte = this.read;
						byte.switch(
							slipESC_END, {
								packet.put(packetSize, slipEND);
								packetSize = packetSize + 1;
							},
							slipESC_ESC, {
								packet.put(packetSize, slipESC);
								packetSize = packetSize + 1;
							},
							{"SLIP encoding error.".warn; packet.postln; }
						)
					},
					{
						packet.put(packetSize, byte);
						packetSize = packetSize + 1;
					}
				);
			}.loop;
		}
	}

	writeSLIP { | bytes, timeout=0.005 |
		this.put(slipEND, timeout);
		bytes.do { |byte|
			byte.switch(
				slipEND, {
					this.put(slipESC, timeout);
					this.put(slipESC_END, timeout);
				},
				slipESC, {
					this.put(slipESC, timeout);
					this.put(slipESC_ESC, timeout);
				},
				{
					this.put(byte, timeout);
				}
			);
		};
		this.put(slipEND, timeout);
	}

	close {
		super.close;
		inputThread.stop;
	}
}