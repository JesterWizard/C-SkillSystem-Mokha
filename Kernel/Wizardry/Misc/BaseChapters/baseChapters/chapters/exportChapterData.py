# Convert raw chapter data hex into EA format.
# The idea is for this to generate copy-chapters once,
# then let the user modify the generated chapters manually as they see fit.
import re

# If you've already repointed chapterData, change these values appropriately!
chapterDataStart = 0x8B0890
chapterDataEnd = 0x8B363C
cleanFE8ROMFileName = "FE8_clean.gba"

# Reads raw hex data of a chapter, formats it into EA format
# and gives it a new eventPLISTid. Essentially, this copies
# the chapter but makes the copy use different events.
def writeChapterdata(input, outputFileName, chapterID, eventPLISTid):

  # "Ooooh, I'm RE, and I'm so special I can't have more than 99 groups. Split your string and re-evaluate your life choices please!"
  searchString1 = r"([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F])"
  searchString2 = r"([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F]) ([0-9|A-F][0-9|A-F])"

  replaceString1 = r"WORD 0x\4\3\2\1\nBYTE 0x\5 0x\6 0x\7 0x\8 0x\9 0x\10 0x\11 0x\12 //PLIST things.\nBYTE 0x\13\nBYTE 0x\14\nBYTE 0x\15\nBYTE 0x\16\nBYTE 0x\17 0x\18 //Initial coordinates.\nBYTE 0x\19\nBYTE 0x\20\nSHORT 0x\22\21\nSHORT 0x\24\23 0x\26\25 0x\28\27 0x\30\29 0x\32\31 0x\34\33 0x\36\35 0x\38\37\nSHORT 0x\40\39 0x\42\41 0x\44\43\nBYTE 0x\45\nBYTE 0x\46 0x\47 0x\48 0x\49 0x\50 0x\51 0x\52 0x\53 0x\54 0x\55 0x\56 0x\57 0x\58 0x\59 0x\60 0x\61 0x\62\nSHORT 0x\64\63 0x\66\65 0x\68\67 0x\70\69 0x\72\71 0x\74\73 0x\76\75 0x\78\77 0x\80\79 0x\82\81 0x\84\83 0x\86\85 0x\88\87 0x\90\89 0x\92\91 0x\94\93 0x\96\95\n"
  #replaceString2 = r"WORD 0x\4\3\2\1 0x\8\7\6\5 0x\12\11\10\9 0x\16\15\14\13\nSHORT 0x\18\17 0x\20\19\nBYTE 0x\21 //event PLIST.\nBYTE 0x\22 //world map event.\nBYTE 0x\23 0x\24 0x\25 0x\26 0x\27 0x\28 0x\29 0x\30 0x\31 0x\32\nBYTE 0x\33 //chapter number.\nBYTE 0x\34 0x\35 0x\36 0x\37 0x\38\nBYTE 0x\39\nBYTE 0x\40\nSHORT 0x\42\41 0x\44\43\nBYTE 0x\45\nBYTE 0x\46\nBYTE 0x\47\nBYTE 0x\48 0x\49\nBYTE 0x\50 0x\51 0x\52"
  replaceString2 = r"WORD 0x\4\3\2\1 0x\8\7\6\5 0x\12\11\10\9 0x\16\15\14\13\nSHORT 0x\18\17 0x\20\19\nBYTE " + str(hex(eventPLISTid)) + r" //event PLIST.\nBYTE 0x\22 //world map event.\nBYTE 0x\23 0x\24 0x\25 0x\26 0x\27 0x\28 0x\29 0x\30 0x\31 0x\32\nBYTE 0x\33 //chapter number.\nBYTE 0x\34 0x\35 0x\36 0x\37 0x\38\nBYTE 0x\39\nBYTE 0x\40\nSHORT 0x\42\41 0x\44\43\nBYTE 0x00\nBYTE 0x\46\nBYTE 0x\47\nBYTE 0x\48 0x\49\nBYTE 0x\50 0x\51 0x\52"

  string1 = input[0:3*96-1]
  string2 = input[3*96:len(input)]

  newString1 = re.sub(searchString1, replaceString1, string1)
  newString2 = re.sub(searchString2, replaceString2, string2)
  newString = newString1 + newString2
  newChapterData = "// Base for chapter " + str(hex(chapterID)) + ".\n\n" + newString
  
  f = open(outputFileName, "w")
  f.write(newChapterData)
  f.close()

def main():
  wmChapterIDs = [0x00, 0x38, 0x02, 0x03, 0x04, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x3D, 0x0D, 0x0E, 0x17, 0x18, 0x3E, 0x1A, 0x1B, 0x0F, 0x10, 0x11, 0x12, 0x13, 0xFF, 0x14, 0x24, 0x2E, 0x39]   # This list is indexed by FE8 world map nodes and contains their corresponding (Eirika mode) chapterIDs.
  wmEventIDs = [1, 2, 3, 4, 5, 6, 8, 9, 11, 12, 14, 15, 16, 17, 18, 19, 21, 22, 24, 25, 26, 27, 28, 29, 31, 32, 34, 35, 37]   # This list is indexed by FE8 world map nodes and contains their corresponding base chapters' eventPLISTids.

  f = open("../../" + cleanFE8ROMFileName, "rb")
  rawChapterData = f.read()
  f.close()
  rawChapterData = rawChapterData[chapterDataStart:chapterDataEnd]
  
  # Convert raw hex to string.
  chapterDataString = ""
  for byte in rawChapterData:
    char = str(hex(byte))[2:4].upper()
    if byte < 0x10:
      char = "0" + char
    chapterDataString += char + " "
  
  for i in range(len(wmChapterIDs)):
    start = wmChapterIDs[i]*444     # Each chapter is size 148, where each byte is made up of
    end = start + 443               # three characters (two hex digits and a space), 148*3=444.
    writeChapterdata(chapterDataString[start:end], str(i)+".event", wmChapterIDs[i], wmEventIDs[i])

main()