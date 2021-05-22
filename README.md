BATTLESHIP
===========
Run with "make run".

Rules
======
Both players place their K ships on a NxN grid. They start with C currency.<br />
They take turns placing bombs on the enemy's grid.<br />
There are 4 types of bombs.<br />
-1x1 bombs. If you manage to hit a ship on the enemy's grid you can continue placing 1x1 bombs until you miss. Does not cost any currency. <br />
-3x3 bombs. Costs 20 currency.<br />
-1xn bombs. Blows up an entire row or column. Costs 2N currency. <br />
-1x1 bombs. Tell you how close is the closest part of a ship using manhattan distance. Costs 20 currency. <br />
Every round, each player's currency count inceases by 5 currency <br />
