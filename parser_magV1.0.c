/* This is an generic payload parser example.
** The code find the payload variable and parse it if exists.
**
** IMPORTANT: In most case, you will only need to edit the parsePayload function.
**
** Testing:
** You can do manual tests to this parse by using the Device Emulator. Copy and Paste the following code:
** [{ "variable": "payload", "value": "0109611395" }]
**
** The ignore_vars variable in this code should be used to ignore variables
** from the device that you don't want.
*/
// Add ignorable variables in this array.
const ignore_vars = [];
var input_pack
/**
 * This is the main function to parse the payload. Everything else doesn't require your attention.
 * @param {String} payload_raw 
 * @returns {Object} containing key and value to TagoIO
 */

function parsePayload(payload_raw) 
{
  try 
  {
    const buffer = Buffer.from(payload_raw, 'hex');
    
    var sum = buffer[7];
    var checker = (buffer[0]+buffer[1]+buffer[2]+buffer[3]+buffer[4]+buffer[5]+buffer[6]);
    checker = checker & 0xFF;
    if (sum != checker)
    {
      var data = 
      [
        {
          variable: 'checksum error', 
          value: buffer[0] 
        }
      ];
      return data;

    }

    var data = 
    [
      { variable: 'address', value: buffer[0]},
      { variable: 'pulses',  value: buffer[2] | (buffer[1]<<8) , unit: 'L' },
      { variable: 'pulses_acum',  value:(buffer[6] | (buffer[5]<<8) | (buffer[4]<<16) | (buffer[3]<<24)), unit: 'L' },
    ];
    return data;


  } catch (e) {return [{ variable: 'parse_error', value: e.message }];}
}
// Remove unwanted variables.
payload = payload.filter(x => !ignore_vars.includes(x.variable));

// Payload is an environment variable. Is where what is being inserted to your device comes in.
// Payload always is an array of objects. [ { variable, value...}, {variable, value...} ...]
const payload_raw = payload.find(x => x.variable === 'payload_raw' || x.variable === 'payload' || x.variable === 'data');
if (payload_raw) 
{
  // Get a unique serie for the incoming data.
  const { value, serie } = payload_raw;

  // Parse the payload_raw to JSON format (it comes in a String format)
  if (value) 
  {
    payload = parsePayload(value).map(x => ({ ...x, serie }));
  }
}
