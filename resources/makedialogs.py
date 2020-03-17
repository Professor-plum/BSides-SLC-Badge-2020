

dialogs = [
		#0:Place holder
		"",
		#1:Intro message
		"Thanks for arriving so quickly detective. As you've probably heard, we've had a number of large circuit breakers fail. Some here suspect fowl play.",
		#2:Intro cont
		"Whatever it is that happened here, I'm hoping you can put it to rest quickly. I'd like to get back to our regular operations ASAP.",
		# *********  Boss dialogs ************
		#3:Init message
		"Have you found anything yet? I'd like to wrap this up soon.",
		#4:Position
		"I run this power station. There isn't anything that happens here without my signing off on it. Seems like these last few weeks have been busier than usual.",
		#5:Damage
		"Like I said, we've had a string of bad luck and a number of our circuit breakers have failed recently. They're not cheap to fix either.",
		#6:Suspects
		"I don't think anyone is behind this, but some of our team doesn't agree. You should ask Bert in the repair yard about his thoughts.",
		#7:Missing
		"Are you sure? I don't think so. If someone were to try and smuggle those out of here we would surely catch them.",
		#8:Phantom
		"The name is slightly familiar, I vaguely remember seeing that name on some spam. They were making some outlandish threats, I just deleted the email.",
		#9:email
		"I knew it! This is all Joe's fault! He opened malware and caused the whole thing! Does this mean you'll be wrapping up the case then?",
		#10:email 2
		"Well please do hurry, I already have enough to deal with.",
		#11:Sabotage
		"All this from that email? Joe will surely pay for the damage he's caused.",
		#12: cake
		"Thank you, with all this commotion I skipped lunch. ...  Ohhh, my stomach isn't feel so well. If you'll excuse me I need to ah, find a restroom. How old was that cake anyway?",


		# ******************** Player dialogs *****************
		# 13:Position
		"What is it you do here exactly?",
		# 14:Damage
		"Can you tell me anything about the ruined circuit breakers?",
		# 15:Suspects
		"Do you know anyone who would be motivated to do something like this?",
		# 16:Missing
		"Apparently some of the backup circuit breakers are missing. Do you know anything about this?",
		# 17:Phantom
		"Have you heard the name PH4N70M before?",
		# 18:email
		"It appears Joe received a malicious email. I have reason to believe he may have clicked the attachment.",
		# 19:ransomware
		"I'm afraid to tell you that I've found ransomware on your network.",
		# 20:goodbye
		"That's all the questions for now.",
		# 21:help
		"Can I do anything to help?",
		# 22:not at fault
		"He's not entirely at fault. Anybody could have fallen for it and he meant no harm. I think there may be more to this mystery, I still have some investigating to do.",
		# 23 recordings
		"Were you able to decrypt the security camera recordings?",
		#24 missing fist
		"Some circuit breakers have gone missing?",
		# 25 blackmail
		"I found the blackmail note you received. Is everything okay?",
		#26
		"Right now, I'm focused on finding the saboteur. I have reason to believe they are one in the same with your blackmailer. If you help me we can bring them to justice.",
		# 27 error screen
		"That screen behind you doesn't look good. Is something wrong?",
		# 28 footage
		"It seems there are 5 other circuit breakers that have been tampered with. We found it on the security camera footage.",
		# 29 final
		"I understand how you feel, but this isn't going to fix anything. Don't do anything you're going to regret. We can still work this out.",
		"",
		"",

		# ****************** Worker dialogs **********************

		# 32: Init
		"I hope this doesn't take long, if you haven't noticed I've got a lot to do.",
		# 33: Posiiton
		"I keep the place afloat. If something breaks down I fix it. But right now the list of broken things is longer than I can keep up with.",
		# 34: Damage
		"Yeah, someone or something keeps causing them to malfunction. I don't see any outward signs of physical damage but internally the circuit is fried. It's really frustrating.",
		# 35:Suspects
		"No idea. All our substations are controlled via fancy SCADA systems. It could have been anyone of those nerds back at the control lab. If you're going to point fingers that's where I'd start.",
		# 36:Missing
		"Yes, three in total have disappeared over the last few weeks. I didn't report it because I just assumed they were around here somewhere.",
		# 37:Missing2
		"It's not like you could just walk out the front door with a circuit breaker and not have someone notice.",
		# 38:Missing3
		"Besides, it's only the old ones that have gone missing. Why not steal the new models? Maybe Becca had them hauled off and just didn't tell anybody.",
		# 39:Phantom
		"Can't say I have.",
		# 40:email
		"I've heard about them viruses. My mother got one on her computer too, we've figured it'd be cheaper just to get a new computer...",
		# 41:ransomware
		"Huh, somebody ain't gonna be too happy 'bout that.",
		# 42:help
		"You could start by getting me a new capacitor from the supply room.",
		# 43:help2
		"This thing is all messed up, someone really did a number to its configuration. Can you help me recalibrate it? I'll need you to adjust the voltage output at Vref",
		# 44:help2b
		"That doesn't seem right, you should try again.",
		# 45:help3
		"Thanks for offering but I think this one is good to go. I'll send it back into the field tomorrow.",
		# 46:goodbye
		"While you're snooping around maybe you can find our missing circuit breakers.",
		# 47:fixed
		"Looks like it's up and running now, thanks. Feel free to scope it out, maybe you'll find something I missed.",
		# 48
		"That's not good. Here's a tool I made to repair all the substations but you'll have to manually scan them with your watch. You'll need to find and scan all 5.",
		# 49: cake
		"No thanks, I've already had enough of that cake.",
		#50: not cap
		"That's not a capacitor. Take this back and grab a new capacitor from the supply room.",
		#51: right cap
		"Thanks, that should be all of the hardware that needs replacing. Now to get it running again.",

		# ******************** Comp person dialogs *****************
		# 52: Init
		"Are you here to take me in?",
		# 53:Position
		"Look, I don't know anything about what happened to the equipment. I only cover the night shift. I just monitor the panel for errors.",
		# 54:Damage
		"No, I don't know what happened. Everything looked fine from my end. No errors reported, then all of a sudden it just broke. The whole thing went offline at once.",
		# 55:Suspects
		"I know the boss doesn't like me much. I think he wants to get rid of me. Maybe he set me up. Or maybe it was Ryan, he hasn't seemed like himself recently.",
		# 56:Missing
		"I promise, I don't! I know they're going to blame me for that too!",
		# 57:Phantom
		"Oh yes, I got an email from them. They offered free movies but it was fake, nothing happened when I opened the file. I'll forward you the email. (PH4N70M.EML added to CASE FILE)",
		# 58: email
		"So, does that mean this whole thing really is my fault? I'm done for!",
		# 59:ransomware
		"I kind of guessed that's what happened to this machine. I knew something was right but I didn't dare say anything with all the other stuff going on.",
		# 60: cake
		"I really don't feel like eating right now.",
		# "61: Error screen"
		"Yeah, that screen started acting up recently. Without it I can't keep an eye on some of the field sites.",

		# ********************* IT person dialogs *********************
		# 62: Init
		"Yes, what can I help you with?",
		# 63:Position
		"I'm the IT guy. I manage all the computers around this place.",
		# 64:Damage
		"I heard about them, seems odd that many would fail around the same time. Kind of reminds me of my luck.",
		# 65: Suspects
		"You know, Becca always had a thing against Bert. Not sure why. But if you were to make him miserable this would be a good way to do it.",
		# 66: Missing
		"Those old things? Who would want them? I remember configuring those things when I first started here. The command line interface on them is horrible.",
		# 67: Phantom
		"Nope. Is that your gaming handle?",
		# 68: Email
		"Well, there goes my weekend. Can you find out if the malware called out to anywhere? If you know the IP address I could probably pull the traffic capture, assuming it hasn't rolled off yet.",
		# 69: Ransomware
		"Well, that explains why his computer is offline. If you can figure out the encryption key I can script up a job to decrypt the security camera footage files.",
		# 70: Wrong IP
		"Hmm, that IP doesn't sound right. Are you sure that's correct?",
		# 71: Right IP
		"Bingo! Got it right here. I'll transfer it over to you. (SAMPLE.CAP added to CASE FILE)",
		# 72: Wrong enc
		"Nope, those settings didn't work. Have any other ideas?",
		# 73: Right enc
		"Nice! I'm going to start decrypting our security camera footage and see what I can find. You should check back in a few.",
		# 74: Not ready
		"Sorry, I haven't seen anything yet.",
		# 75: Final
		"Yes but it isn't good. It appears the same figure that tampered with this breaker also messed with the 5 other substations. Bert isn't going to like this.",
		# 76: Cake
		"No way! Do you know how old that cake is?",
		# 77 - 81
		"",
		"",
		"",
		"",
		"",

		#************************  Girl dialogs  ********************************
		# 82: Init
		"Hello darling, can I help you?",
		# 83:Position
		"I'm the administrative assistant. I keep this place running, I do everything nobody else wants to do.",
		# 84:Damage
		"Yes I heard about that. Who would want to destroy power equipment? Don't they know that could cause power outages? People would panic if that started happening.",
		# 85:Suspects
		"I got an email the other day from someone named PH4N70M. They made some scary threats if their demands weren't met. I sent it to management but haven't heard anything.",
		# 86:Missing
		"Thank heavens! I've been trying to convince Bert to get rid of those things for months. They are such an eye sore!",
		# 87: phantom
		"He's the one that sent me the threatening email the other day. People like that are worse than a snake in the grass!",
		# 88: cake
		"Bless your heart, I made that cake last week!",
		# 89 - 91
		"",
		"",
		"",


		#************************  engineer dialogs  ********************************
		# 92: Init
		"Good afternoon",
		# 93:Position
		"I'm an electrical engineer. I work on the equipment and design new subsystems. Those new circuit breakers in the yard are part of the upgrade I'm working on.",
		# 94:Damage
		"Yeah, that old equipment is failing faster then any of us expected. Because of that I've been working overtime to get the new system up and running.",
		# 95:Suspects
		"Suspects? You think someone is causing the equipment to fail? You do realize squirrels are the largest cause of power outages right? How do you know it isn't one of them?",
		# 96:Missing
		"uhh... missing? I'm surprised anyone noticed they were gone. Maybe... someone took them to the dump already.",
		# 97: phantom
		"Who now? Are you asking if I believe in ghosts? I'm not really into that sort of thing.",
		# 98: cake
		"Thanks but no thanks. I'm trying to get in shape and that cake has been sitting on the table tempting me all week.",
		# 99  blackmail 1
		"... Oh that. No, I'll be fine. I gave that creep those old circuit breakers to get him off my case. You can turn me in just don't mention that note. I just want that creep to disappear.",
		# 100 blackmail 2
		"Okay yeah. If it means this guy will be put away I'm in. I moved those circuit breakers to the service tunnels entrance. I don't know anything more, I promise. Here's how you get there.",
		#101
		"",



		#*************************  Enemy dialogs   ******************************
		#102 //Initial run in
		"So you're the one who's been snooping around. I'm impressed you've found my laboratory. Now it's time for your first lesson here, to mind your own business!",
		#103 //warning (worker)
		"Woah! Thanks for putting out the fire, it had me pinned back there. That freak threw a molotov and the fire grew intense fast. Must have susprised him too, I think it trapped him in the supply room.",
		#104 //warning 2
		"If you plan on confronting him be prepared to defend yourself. He's not going to hold back. Sock him one in the chops just for me.",
		#105 //final1
		"I thought I took care of you. Why couldn't you just stay out of this. Don't you see what's happeneing here? I am trying to save everyone.",
		#106 //final2
		"The people of this city have become so distracted, always consuming media, never thinking for themselves. I was going to solve all that, put and end to their enslavement.",
		#107 //final 3
		"I'm done playing games. This time I'm going end to your snooping around for good. (He grabs the wrench and begins to swing it. You realize that if you're going to do something now is your last chance!)",		
]

with open("dialogs.bin", 'wb') as f:
	for i in range(128):
		d=""
		if i < len(dialogs):
			d = dialogs[i]
		f.write(d)
		f.write('\0'*(256-len(d)))

