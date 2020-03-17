/*
 * items.c
 *
 *  Created on: Jan 9, 2020
 *      Author: plum
 */


#include "items.h"
#include "flags.h"

/*typedef struct {
	char* name;
	char* description;
	char* takemsg;
	char* openmsg;
	uint8_t takeflag, openflag;
} Item;*/

const Item items[] = {
		{"Key card", "Ah, the managers access card. It is just begging to be borrowed.", "You pick up the key card. With this you now have access to the back office.", 0, FLAG_KEYCARD, 255},
		{"Desk", "A nice mahogany desk. There are a pile of papers on the edge but otherwise it looks like not much gets done here.", 0, "Opening the larger drawers reveal more pointless paperwork, invoices and bills. In the smaller drawer you find some packing tape, and a key card.", 255, FLAG_DESK},
		{"Papers", "Looks like your typical paper pusher paper work. The number of bills in the pile does seem a bit overwhelming though.", "None of these papers seem relevant", 0, 255, 255},
		{"Computer", "Obviously the manager has to have a computer but you get the impression this one doesn't get used much.", 0, "That doesn't seem like a good idea", 255, 255},
		{"Charles", "The guy looks like a used car sales man. You get the feeling he's more concerned about the down time caused by the current situation than anything else.", 0, 0, 255, 255},
		{"Packing Tape", "", "You pick up the tape, just in case", 0, FLAG_TAPE, 255},
		{"Joe", "This guy looks like a dog who knows he's in trouble. He has a nervous twitch, darting eyes and finger nails bitten down to stubs.", 0, 0, 255, 255},
		{"Desktop computer", "This computer has seen better days. It is currently stuck on a BIOS boot screen. Apparently the hard drive is completely corrupt.", 0, 0, 255, 255},
		{"Wall monitors", "The monitors display various information about different systems. However the center large monitor is stuck on some unfamiliar error screen.", 0, 0, 255, 255},
		{"Coat rack", "A small coat rank in the corner of the room, doesn't appear to get much use. There is a single coat hanger on the bar.", "It is already open", 0, 255, 255},
		{"Coat hanger", "A simple wire coat hanger", "You pick up the coat hanger", 0, FLAG_HANGER, 255},
		{"Bert", "A brusque man who seems resentful about something. He's currently working on repairing some large electrical equipment. He also could use some suspenders.", 0, 0, 255, 255},
		{"Circuit breaker", "You've seen one of these before at a power substation. It's about the size of a phone booth with large power cables coming out of the top. There is also service panel on the front.", 0, "Behind the service panel you see a console access port. (You can now access this console port via the USB serial port on the watch itself. 115200bps)", 255, FLAG_PANEL},
		{"Service panel", "There is a service panel on the front of the circuit breaker. ", 0, "You'll need a screwdriver to open this", 0, 255},
		{"Fire ext cabinet", "You know the kind, it contains a fire extinguisher behind a thin piece of glass. In case of emergency...", 0, "There is no nice way to open it. You'll have break it.", 255, 255},
		{"Extinguisher", "A device to extinguish fires... Why are you hesitating, grab it quick!", "You grab the fire extinguisher", 0, FLAG_EXTINGUISH, 255},
		{"Becca", "This lady looks like a small southern belle. She's browsing a fashion magazine and seems pretty chill given the current situation.", 0, 0, 255, 255},
		{"Radio", "An old FM radio. It doesn't seem to be working. The collapsible antenna is broken off and the batteries are dead.", "No sense in hauling a broken radio around.", 0, 255, 255},
		{"Bookshelf", "A large bookshelf full of technical manuals and thick text books. ", "Let's be honest, most the topics in these books are well over your head.", "Nope not a single book on here worth opening, that is unless you have trouble falling asleep.", 255, 255},
		{"Cake", "A half eaten strawberry cream cake. Was probably pretty delicious when it was fresh.", "You grab a piece of cake", 0, FLAG_CAKE, 255},
		{"Fridge", "An office fridge complete with reminders and notices taped to the front.", 0, "There are a few old lunches inside. None of them look worth taking.", 255, 255},
		{"Screwdriver", "It is a screwdriver that was on top of the bookshelf. The kind TSA would confiscate.", "You pickup the screwdriver, \"This will come in handy\"", 0, FLAG_SCREW, 255},
	    {"Drew", "An obvious intellectual, he seems to be in his element here in the server room. He is working on his laptop from a chair cramped in the corner.", 0, 0, 255, 255},
		{"Server rack", "A large rack of servers, complete with blinking lights and loud fans.", 0, 0, 255, 255},
		{"network cabinet", "Network switches with a mess of cables going every which way. Could definitely use some organization.", 0, 0, 255, 255},
		{"Cable spool", "A large spool of electrical cable, industrial strength.", 0, 0, 255, 255},
		{"Batteries", "AA batteries, some generic knockoff brand.", "You grab the batteries", 0, FLAG_BATTERIES, 255},
		{"Wrenches", "Oversized wrenches, apparently for large equipment. They could belong to Paul Bunyan. ", "You really don't want to carry these things around.", 0, 255, 255},
		{"Pipes", "A lot of different pipes run down here. Some appears to be for water, others are electrical conduits.", 0, 0, 255, 255},
		{"Tunnel", "There is a large dark passage way. It may be worth exploring but you are going to need a light source of some kind.", 0, 0, 255, 255},
		{"Puddle", "A pretty oily water puddle right in front of you. Might be worth stepping over this one.", 0, 0, 255, 255},
		{"Dresden", "A balding man in his late 30s, he looks perplexed about something.  He's sitting at the large table reviewing some blueprints of some kind.", 0, 0, 255, 255},
		{"Equipment", "A microscope, soldering iron, some probes. Lots of electrical tools here. You notice a clear finger print on the glass of the microscope", "It is probably best not to disturb the evidence", 0, 255, 255},
		{"Circuit boards", "A few circuit boards and other electrical parts. These could easily be from those circuit  breakers. Looks like someone has been trying to pull the firmware off them.", 0, 0, 255, 255},
		{"Digital photo", "The display switches between photos of what looks like Dresden's family. There is one photo that doesn't seem to belong. You download that photo. (CHESS.JPG added to CASE FILE)", "The display switches between photos of what looks like Dresden's family. There is one photo that doesn't seem to belong. You download that photo. (CHESS.JPG added to CASE FILE)", 0, 255, 255},
		{"Lock box", "This is one of those new digital lock boxes. Only a specific motion will unlock the box.", 0, "You don't see a way you can force it open. There must be a trick to opening it.", 255, 255},
		{"Fingerprints", "", 0, 0, 255, 255},
		{"Meeting Table", "A large oval table used for meetings.", 0, 0, 255, 255},
		{"Blueprints", "They look to be electrical in nature although you can't really determine any specific details on them.", "Dresden won't allow that.", 0, 255, 255},
		{"Chairs", "A bunch of nice leather chairs all surrounding the table.", 0, 0, 255, 255},
		{"Crumpled note", "A small piece of paper crumpled up on the floor. Looks like there is some writing on it", "You pick up the note. (PUZZLE.TXT added to CASE FILE)", 0, FLAG_PUZZLE, 255},
		{"Flashlight", "", 0, 0, 255, 255},
		{"Workbench", "A makeshift workbench complete with desk lamp. Still not the most ideal place to do research but I guess if you want to be left alone you can't beat it.", 0, 0, 255, 255},
		{"Circuit breakers", "Here are the old circuit breakers that Dresden brought down. They are mostly intact but appear to be missing the logic boards.", 0, 0, 255, 255},
		{"Desk", "The desk is locked up tight. Not a single drawer will open.", 0, 0, 255, 255},
		{"Terminal", "This appears to be a terminal to the cites power grid. The screen is sitting at a login prompt.", 0, 0, 255, 255},
		{"Tool box", "A tool box with a label on the side \"Electrical components\"", 0, "Inside the tool box you see three different types of electrical components. A torus shape with a wire coil around it, a cube shape with 5 legs, and a cylinder shape with 2 legs.", 255, 255},
		{"Electrical part", "", 0, 0, 255, 255},
		{"Steel Bars", "Solid steel bars, there is a door here too. It appears to be held shut via a electric magnetic lock", 0, "It won't open, it is locked shut", 255, 255},
		{"Wiring", "There is some wiring running from the door lock along the wall. There are four wires inside red, blue, green and yellow. Perhaps if you jump these it will short out the lock.", 0, 0, 255, 255},
		{"Fire", "An electronics fire has spread from the transformers to the circuit breaker. If you don't stop it soon it will get out of control quickly!", 0, 0, 255, 255},
		{"Fire ext cabinet", "The cabinet where the fire extinguisher was. Now nothing but broken glass.", 0, 0, 255, 255},
		{"Ruined equipment", "The burnt remains of some rather expensive electrical equipment. Too bad you spent all that effort to repair it.", 0, 0, 255, 255},
};
