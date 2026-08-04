# Changelog

## v1.0.19
- A recording is no longer lost when you leave the level. It could only be saved from the STOP
  button in the pause menu, but the way a practice session actually ends is by quitting - so the
  one thing everybody does was the one path that threw the whole run away without saying anything.
  Leaving while recording now asks whether to keep it

## v1.0.18
- Silicate macros now read correctly. The decode was always fine; the mod was throwing away the
  tick rate the file declared. Anything above 2000 ticks a second was replaced with 240, and
  Silicate records at the physics rate - one real file declares 2229, so a 59-second run was being
  spread over 552 seconds and every cue but the first fifteen landed past the end of the level
- The same ceiling was doing the same thing to Mega Hack files: one declaring 3500 was stretched
  from 70 seconds to 1027
- Dual Mega Hack macros were half-destroyed. The player number sits in the byte immediately after
  the hold flag, and the two were being read together as one number, so every player-two press
  registered as a release - 17,565 records out of 35,918 on one real file
- Dual zBot macros were merged into a single stream, because the check for player two looked for a
  character that no zBot file has ever contained. On one real macro that swallowed 3,400 presses
- Left and right in platformer macros were being treated as jumps, which cut jump cues short
- Macros recorded with iCreate Pro were being read as if they were entirely player two. It marks
  every input of an ordinary single-player run that way, where every other bot marks none of
  them, so the guide ended up hung off the wrong icon. A macro with no player-one input at all
  cannot be a dual, so it is now read as the single-player recording it is
- A macro that will not load now says so, and says whether nothing recognised it or it simply
  contained no inputs, instead of failing silently

## v1.0.17
- A Start Pos you have never used before now starts from what the ones around it already know,
  instead of from nothing. Until now only the exact spawn you had already ground out was correct;
  a fresh one was as wrong as it ever was - a tester's log caught one 71 frames out with all seven
  of his presses too far off to even be graded, because the guide needs ten before it will commit
  to a correction
- It only fills in between Start Positions it has actually measured, and only reaches a little way
  past the outermost ones. The error is not proportional to how deep you are - on one level it was
  6 frames at two minutes in and 77 at seven - so a long reach would be inventing a number rather
  than reading one. Past that limit it says nothing and behaves exactly as before

## v1.0.16
- The guide now remembers the Start Pos correction it worked out, instead of learning it again
  from scratch every time. It was already getting the right answer - a tester's log shows it
  finding the same number, to the frame, eleven times across two sessions - and throwing it away
  each time the Start Pos changed. Relearning cost about ten presses of a badly wrong guide on
  every single switch. Now the first press at a Start Pos you have already used is as good as the
  tenth, this session or next week
- The correction is kept per Start Pos and used nowhere else. v1.0.15 assumed the error grew
  evenly with how deep into the level you were and stretched one measurement across the whole
  level; a sweep of fifteen Start Positions showed that is not true - the error sat within six
  frames from 25 seconds to two minutes in, then reached 77 frames by seven minutes. Stretching
  the deep measurement backwards would have spoiled the middle of the level to fix the end of it
- v1.0.15's version of this never actually did anything on levels saved locally or copied into
  the editor, because it was filed under the level's online ID and those do not have one. It
  failed silently. Fixed, and the log now says which key it used and what it loaded
- Runs from 0% are untouched by all of this

## v1.0.15
- Fixes the guide being badly out when you start from a Start Pos deep in a level, and replaces
  the v1.0.14 measurement below, which was wrong. A tester's log settled it: starting at 80% into
  a seven-minute level, every press landed 78 frames from where the mod thought the click was,
  and eight out of ten presses missed by too much to even be graded
- The cause is that Geometry Dash's own model of when the player reaches a given point is very
  slightly slow - about 0.08% - and 0.08% of seven minutes is a third of a second. That is why
  playing from 0% always looked fine and only a deep Start Pos looked broken: the error is zero
  at the start and grows the further in you go
- The mod already worked this out from your own presses and corrected it - and it was right every
  time, to the frame - but it threw the answer away every time you changed Start Pos, and paid
  another ten presses of a badly wrong guide to learn the same number again. It now keeps it. The
  first press at a new Start Pos is already correct, and so is the first press next time you open
  the level
- The correction is kept as a rate rather than an offset, so it is right everywhere in the level
  rather than only near where it was measured - and it is stored per level, so it can never be
  applied to a level it was not measured on
- The same correction now also moves the marks themselves, which fixes them sitting further and
  further behind you the deeper into a level you get - about three blocks by seven minutes in -
  on runs from 0% as well

## v1.0.14
- The guide no longer drifts later and later the further into a level you get. It was working
  from the speed constants everyone quotes for Geometry Dash, and those constants are slightly
  low - but by a different amount on different machines. On one identical level a Windows PC
  covered 311.9 units a second and an Apple Silicon Mac covered 312.3, against a table that says
  311.6. Small, but it accumulates: the same gap is exactly the drift both machines were
  reporting, and it is the whole of the error in every projection the mod makes
- So the mod measures it instead of assuming it. It watches how far you actually move against
  how far it expected you to, and corrects itself. Ten seconds of play is enough, it settles and
  then sits still, and it is remembered - it is a property of your machine, not of the level, so
  the next level starts already correct. Dashes, teleports and slopes are ignored rather than
  averaged in
- Thanks to the tester who found this in the debug logs and worked out it was the speed constant.
  He was right, and his own numbers are why it is measured rather than hardcoded - the value that
  would have fixed his Mac would have made a Windows PC worse

## v1.0.13
- The macro download button on a level page no longer lands on top of the like/rate button. It
  now looks at what is already on the page and finds a free spot, so it also stays clear of
  buttons other mods and texture packs add

## v1.0.12
- The master switch is the first thing on the settings page now, instead of the last row of
  EXTRAS underneath the highway sliders. It is the control people reach for most and it was in
  the least reachable place
- Grading your releases is a setting of its own and off by default - see v1.0.10
- Two different toggles were both called "Stats readout". The one that shows PERFECT / +2 / -1
  after each press is now called "Trainer feedback", which is what it does

## v1.0.11
- Sign-in now tells you why it could not reach the server instead of just that it could not. A
  refused connection asks about your firewall, a failed secure connection points at antivirus
  HTTPS scanning, and a timeout says which is the usual cause. Several people have hit this and
  the old message gave them nothing to try

## v1.0.10
- The mod grades your releases, not just your presses. In ship, UFO, wave, robot and swing how
  long you hold is the input, so you could press on exactly the right frame, let go wrong, die,
  and still be told PERFECT - because nothing ever looked at the half that killed you. Now you
  get "LET GO 7 EARLY" or "HELD 4 TOO LONG" when the hold was wrong. Turn it on under
  "Grade releases too"; it stays quiet on taps and on holds you got right
- The guide is no longer thrown off when you start from a Start Pos deep in a level. It was
  reading the clock from the game's own model of the level, which drifts the further in you
  are - measured at a third of a second on a real report, which is about 34 frames. It now
  works the offset out from your own presses instead

## v1.0.9
- Auto-calibrate is gone rather than merely off. It shifted the cue to match how you had been
  clicking, which meant the mark moved between attempts, and a mark that moves is worse to play
  against than one that sits still even when the still one is slightly out. If you want a fixed
  offset, "Cue lead" still does that and stays where you put it

## v1.0.8
- Auto-calibrate is off again by default. It keeps adjusting while you play, so the cue can
  shift slightly between attempts, and that is worse than a cue that sits still even when the
  still one is a fraction late. The cue is back on the macro's exact timing
- If you did want the compensation, turn it back on in settings - nothing about it changed.
  For a fixed offset that never moves, use "Manual lead" instead

## v1.0.7
- Fixes the guide feeling late on every macro. Auto-calibrate is now on by default. The game
  draws a frame, your screen shows it, and your click travels back - all of that takes time, and
  it is different on every screen and every machine. Nothing was cancelling it before, so the
  cue sat exactly on the macro's timing and you were always a little behind it no matter how
  well you played. Calibration measures the gap from your own presses and moves the cue to
  match. If your timing already had no bias it changes nothing, so it cannot make a setup that
  works today any worse
- You can still turn it off if you want the cue sitting exactly on the macro's timing
- Macros now load through your Click Indicators account instead of being fetched anonymously.
  Nothing changes while you are signed in. If you are signed out the macro list now tells you
  so, rather than loading anyway

## v1.0.6
- Now runs on macOS and Android as well as Windows. One download covers all of them - Intel
  Macs, Apple Silicon, and both 32 and 64 bit Android
- On Android, install it from inside the game: Geode's mods screen has an "Install from file"
  button that opens your file picker. No computer needed
- Free camera and the click editor need a keyboard, so they are desktop only. The setting says
  so now instead of telling phone players to press WASD

## v1.0.5
- Turn the guide off for part of a level, so you can practise a section unaided while the rest
  still shows. Two new settings, "Hide guide from" and "Hide guide to" - set 0 and 20 to leave
  the opening bare, or 20 and 50 to blank out a section in the middle
- Mega Hack macros recorded across a restart no longer scatter phantom clicks over the start of
  the level. A .mhr holds the whole recording session, and everything before the last restart
  belonged to an attempt that ended
- A failed macro download now says what went wrong instead of a bare error number. Timed out,
  blocked by a firewall, antivirus interfering, macro removed from the server and rate limiting
  all read differently now, because each needs something different from you. Downloads also
  wait longer before giving up

## v1.0.4
- The password box now accepts every character you can type. It was quietly ignoring spaces,
  accented letters and several symbols, so if your password contained one of them you could
  type into the box and nothing appeared - with no way to tell why. Signing in on the website
  worked fine, which made it stranger still

## v1.0.3
- Your licence now works on one device at a time. Signing in inside the game on another
  PC moves it there and signs the old one out, so switching machines needs nothing from
  the website - and the machine that lost it is told why, rather than just stopping
- The licence is rechecked when the game starts and every half hour of play, instead of
  once a day. Losing your connection still does not sign you out
- Searching for a level in the macro window no longer breaks Geometry Dash's own level
  lists. The search took over a slot the whole game shares and then dropped it, so a list
  that was loading when you opened the window could sit on its spinner forever
- The input hook now declares where it sits relative to other mods, instead of landing
  wherever the load order happened to put it. It could previously grade and record a click
  that another mod had already swallowed - a press the game never acted on
- Macro files are now hardened against malformed and hostile input. A corrupt or
  crafted file could previously hang or crash the game on entering a level, and a
  downloaded file could be written outside the mod's own folder
- A server outage no longer signs you out. The licence check now only acts on an
  answer the server actually authored, and a clock that jumps no longer locks you out
- Safe mode: the first attempt of a level is no longer miscounted, and a level whose
  macro fails to load no longer keeps the previous level's macro and voids your runs
- No more white cube over your icon at the press moment. It was meant as a brief
  glow, but it drew as a hard square covering the one thing you are watching
- The safe mode checkbox no longer unticks itself before you have answered the
  confirmation - it showed the opposite of the actual setting until the settings
  screen was reopened
- "Guide lines in Classic only" now hides the hold bar too, instead of leaving it
  drawn on the level after you asked for the lines to stop
- Free camera, recorder and paused-editor state no longer carry into the next level

## v1.0.2
- Cues are placed from where you are, not integrated from the level start. That
  error grew with distance - a couple of units early in a level, over seventy by
  the end - which is why a start pos deep in a level looked wrong while the
  opening looked fine
- Each cue is worked out once and then holds still, instead of creeping forward
  and settling as you approach it
- Start pos: the clock is corrected against position/time measured from real
  play, which is the only thing that sees the small residual in GD's own model.
  Levels you have played from the start are exact; elsewhere the guide anchors
  itself within a couple of clicks and the HUD says which it is doing
- A cue stays until you have actually passed it, rather than vanishing a few
  frames early
- Mirror portals no longer hide the timing window and hold bar
- With more than one macro saved for a level, the most complete one is chosen,
  and the same one every launch - it used to be whichever the disk returned first
- The guide no longer falls apart at high speedhack rates

## v1.0.1
- Start pos: the guide is aligned from the level and the spawn point rather than
  from where you happen to be standing. Practice checkpoints no longer push the
  cue further ahead every time you die, and when the alignment can't be worked
  out the guide turns off and says so instead of cueing the level's opening
- Highway: set the lane's position and size, and turn off the lane lines
- Pulse: grows out to a pair of fixed marks so you can see how far it has to go,
  and passes behind your icon instead of covering it
- Converge: upcoming clicks fade in behind the main indicator, so you can see the
  rhythm coming rather than one press at a time
- Recording from a start pos saved a macro on the wrong timebase; it no longer does

## v1.0.0
- Five cues: Ring, Classic, Converge, Pulse, Highway
- Highway is a single-lane Guitar Hero readout: notes fall toward a strike bar and
  holds are drawn as long notes, so you can see the rhythm coming
- Safe mode (on by default) - nothing counts while it's on
- Auto-calibration that learns your timing and holds it steady during an attempt
- Record your own runs, including practice mode and at reduced speed
- Reads Silicate, GDR2, GDR, zBot, Mega Hack, ReplayBot, TASBOT, Echo, DDHOR, xBot, JSON
- Per-level macro downloads plus loading your own files
- Four cue sound packs, separate release tone for holds
- Per-gamemode toggles, opacity + colour
