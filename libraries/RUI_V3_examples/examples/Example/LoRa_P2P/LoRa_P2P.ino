long startTime;
bool rx_done = false;
double myFreq = 868000000;
uint16_t sf = 12, bw = 125, cr = 0, preamble = 8, txPower = 22;

void
hexDump (uint8_t * buf, uint16_t len)
{
  char alphabet[17] = "0123456789abcdef";
  Serial.
    print (F
	   ("   +------------------------------------------------+ +----------------+\n"));
  Serial.
    print (F
	   ("   |.0 .1 .2 .3 .4 .5 .6 .7 .8 .9 .a .b .c .d .e .f | |      ASCII     |\n"));
  for (uint16_t i = 0; i < len; i += 16)
  {
    if (i % 128 == 0)
      Serial.
	print (F
	       ("   +------------------------------------------------+ +----------------+\n"));
    char s[] =
      "|                                                | |                |\n";
    uint8_t ix = 1, iy = 52;
    for (uint8_t j = 0; j < 16; j++)
    {
      if (i + j < len)
      {
	uint8_t c = buf[i + j];
	s[ix++] = alphabet[(c >> 4) & 0x0F];
	s[ix++] = alphabet[c & 0x0F];
	ix++;
	if (c > 31 && c < 128)
	  s[iy++] = c;
	else
	  s[iy++] = '.';
      }
    }
    uint8_t index = i / 16;
    if (i < 256)
      Serial.write (' ');
    Serial.print (index, HEX);
    Serial.write ('.');
    Serial.print (s);
  }
  Serial.
    print (F
	   ("   +------------------------------------------------+ +----------------+\n"));
}

/*
  typedef struct rui_lora_p2p_revc {
  // Pointer to the received data stream
  uint8_t *Buffer;
  // Size of the received data stream
  uint8_t BufferSize;
  // Rssi of the received packet
  int16_t Rssi;
  // Snr of the received packet
  int8_t Snr;
  } rui_lora_p2p_recv_t;
*/
void
recv_cb (rui_lora_p2p_recv_t data)
{
  rx_done = true;
  if (data.BufferSize == 0)
  {
    Serial.println ("Empty buffer.");
    return;
  }
  char buff[92];
  sprintf (buff, "Incoming message, length: %d, RSSI: %d, SNR: %d",
	   data.BufferSize, data.Rssi, data.Snr);
  Serial.println (buff);
  hexDump (data.Buffer, data.BufferSize);
}

void
send_cb (void)
{
  Serial.printf ("P2P set Rx mode %s\r\n",
		 api.lorawan.precv (65534) ? "Success" : "Fail");
}

void
setup ()
{
  Serial.begin (115200);
  Serial.println ("RAKwireless LoRaWan P2P Example");
  Serial.println ("------------------------------------------------------");
  delay (2000);
  startTime = millis ();
  Serial.println ("P2P Start");
  Serial.printf ("Hardware ID: %s\r\n", api.system.chipId.get ().c_str ());
  Serial.printf ("Model ID: %s\r\n", api.system.modelId.get ().c_str ());
  Serial.printf ("RUI API Version: %s\r\n",
		 api.system.apiVersion.get ().c_str ());
  Serial.printf ("Firmware Version: %s\r\n",
		 api.system.firmwareVersion.get ().c_str ());
  Serial.printf ("AT Command Version: %s\r\n",
		 api.system.cliVersion.get ().c_str ());
  Serial.printf ("Set Node device work mode %s\r\n",
		 api.lorawan.nwm.set (0) ? "Success" : "Fail");
  Serial.printf ("Set P2P mode frequency %3.3f: %s\r\n", (myFreq / 1e6),
		 api.lorawan.pfreq.set (myFreq) ? "Success" : "Fail");
  Serial.printf ("Set P2P mode spreading factor %d: %s\r\n", sf,
		 api.lorawan.psf.set (sf) ? "Success" : "Fail");
  Serial.printf ("Set P2P mode bandwidth %d: %s\r\n", bw,
		 api.lorawan.pbw.set (bw) ? "Success" : "Fail");
  Serial.printf ("Set P2P mode code rate 4/%d: %s\r\n", (cr + 5),
		 api.lorawan.pcr.set (0) ? "Success" : "Fail");
  Serial.printf ("Set P2P mode preamble length %d: %s\r\n", preamble,
		 api.lorawan.ppl.set (8) ? "Success" : "Fail");
  Serial.printf ("Set P2P mode tx power %d: %s\r\n", txPower,
		 api.lorawan.ptp.set (22) ? "Success" : "Fail");
  api.lorawan.registerPRecvCallback (recv_cb);
  api.lorawan.registerPSendCallback (send_cb);
  Serial.printf ("P2P set Rx mode %s\r\n",
		 api.lorawan.precv (3000) ? "Success" : "Fail");
  // let's kick-start things by waiting 3 seconds.
}

void
loop ()
{
  uint8_t payload[] = "payload";
  if (rx_done)
  {
    rx_done = false;
    Serial.printf ("P2P send %s\r\n",
		   api.lorawan.psend (sizeof (payload),
				      payload) ? "Success" : "Fail");
  }
  delay (500);
}
