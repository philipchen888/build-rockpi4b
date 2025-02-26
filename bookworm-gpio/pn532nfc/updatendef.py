"""
    Updates a sector that is already formatted for NDEF (using
    mifareclassic_formatndef.py for example), inserting a new url

    To enable debug message, define DEBUG in nfc/pn532_debug.h
"""

import sys

if sys.argv[1] == "i2c":
    sys.exit( 0 )
else:
    from pn532_spi import *

    # We can encode many different kinds of pointers to the card,
    # from a URL, to an Email address, to a phone number, and many more
    # check the library header .h file to see the large # of supported
    # prefixes!
 
# For a http:#www. url:
url = "elechouse.com"
ndefprefix = NDEF_URIPREFIX_HTTP_WWWDOT

# for an email address
# url = "mail@example.com"
# ndefprefix = NDEF_URIPREFIX_MAILTO

# for a phone number
# url = "+1 212 555 1212"
# ndefprefix = NDEF_URIPREFIX_TEL


def setup():
  begin()

  versiondata = getFirmwareVersion()
  if not versiondata:
    print("Didn't find PN53x board")
    raise RuntimeError("Didn't find PN53x board")  # halt

  # Got ok data, print it out!
  print("Found chip PN5{:02x} Firmware ver. {:d}.{:d}".format((versiondata >> 24) & 0xFF, (versiondata >> 16) & 0xFF,
                                                              (versiondata >> 8) & 0xFF))
  # Set the max number of retry attempts to read from a card
  # This prevents us from waiting forever for a card, which is
  # the default behaviour of the PN532.
  setPassiveActivationRetries(0xFF)

  # configure board to read RFID tags
  SAMConfig()

  print("Waiting for an ISO14443A Card ...")

def loop():
  authenticated = False               # Flag to indicate if the sector is authenticated

  # Use the default NDEF keys (these would have have set by mifareclassic_formatndef.pde!)
  keya = bytearray([ 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 ])
  keyb = bytearray([ 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 ])

  print("Place your NDEF formatted Mifare Classic card on the reader to update the NDEF record")
  # Wait for user input before proceeding

  # Wait for an ISO14443A type card (Mifare, etc.).  When one is found
  # 'uid' will be populated with the UID, and uidLength will indicate
  # if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = 0
  while not success:
      time.sleep(.0005)    # sleep 0.5 ms
      success, uid = readPassiveTargetID(PN532_MIFARE_ISO14443A_106KBPS)

  if (success):
    # Display some basic information about the card
    print("Found a card!")
    print("UID Length: {:d}".format(len(uid)))
    print("UID Value: ", end =" ")
    PrintHex(uid)
    print("")
    
    # Make sure this is a Mifare Classic card
    if (len(uid) != 4):
      print("Ooops ... this doesn't seem to be a Mifare Classic card!")
      return True

    # We probably have a Mifare Classic card ... 
    print("Seems to be a Mifare Classic card (4 byte UID)")

    # Check if this is an NDEF card (using first block of sector 1 from mifareclassic_formatndef.pde)
    # Must authenticate on the first key using 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7
    success = mifareclassic_AuthenticateBlock (uid, 4, 0, keyb)
    if (not success):
      print("Unable to authenticate block 4 ... is this card NDEF formatted?")
      return True

    print("Authentication succeeded (seems to be an NDEF/NFC Forum tag) ...")

    # Authenticated seems to have worked
    # Try to write an NDEF record to sector 1
    # Use 0x01 for the URI Identifier Code to prepend "http:#www."
    # to the url (and save some space).  For information on URI ID Codes
    # see http:#www.ladyada.net/wiki/private/articlestaging/nfc/ndef
    if (len(url) > 38):
      # The length is also checked in the WriteNDEFURI function, but lets
      # warn users here just in case they change the value and it's bigger
      # than it should be
      print("URI is too long ... must be less than 38 characters!")
      return True

    print("Updating sector 1 with URI as NDEF Message")
    
    # URI is within size limits ... write it to the card and report success/failure
    success = mifareclassic_WriteNDEFURI(1, ndefprefix, url)
    if (success):
      print("NDEF URI Record written to sector 1")
      print("")      
    else:
      print("NDEF Record creation failed! :(")

    print("Done!")
    return True
    
  else:
    print("Timed out waiting for card")
    return True


if __name__ == '__main__':
    setup()
    found = loop()
    while not found:
        found = loop()
