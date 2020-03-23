# A Painfully Minimal vi Clone

Impliments a modal vi-style editor, with only hjkl movement keys, plus a `q` quit command, in 150 lines of basic C++.  I suppose the ANSI escapes are arcane, but they always are.  When I say minimal, I mean it; there is no saving or loading, and no scrolling, dealing with long lines, or exiting cleanly.  This is more of an example of how simple the core of a text editor can be, rather than a polished and usable product, and of course a good opertunity to dabble in C++.

While it may not be performant on massive text files, for most source-code-sized inputs, the strategy of simply moving the entire text buffer for every character is perfectly workable on a modern computer, and *much* simpler to code.

To be clear, the commands are:

|Mode   |Key      |Action                                        |
|-------|---------|----------------------------------------------|
|Insert |Backspace|Delete the character to the left of the cursor|
|Insert |Escape   |Switch to command mode                        |
|Command|h        |Move left                                     |
|Command|j        |Move up                                       |
|Command|k        |Move down                                     |
|Command|l        |Move right                                    |
|Command|i        |Switch to insert mode                         |
|Command|q        |Quit the program                              |
