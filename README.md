# Cryptocurrency Analyzer program
I write this program to get rich soon. but after loosing $500 in the market I find that it needs more time and energy and skill to monitor crypto market and earn money. 
Crypto market is very fluctuating and real expert guys can earn money from it.
Anyway I decided to develop this program to monitor the price of some  cryptoz and when those cryptoz start to grow in price I buy them and after earning 5 percent or above, I sell them. 
I get a key from www.cryptocompare.com and use the API of them to get the price of those coins.
To have flexibility to add or remove coins, I use a config.ini file which has 3 keys:
Config.ini sample contents:

[GENERAL]
COINS=WING,AVAX,UNFI,BTC,ETH,AXS,ALICE,BLZ,GRT,TRB,BABYDOGE,SHIB,SOL,CHZ
KEY=get your code from www.cryptocompare.com and put here
VIP=WING,AVAX,UNFI,SOL,CHZ

- add your favorite coins to COIN key like above ( all upper case)
- get an API  key from www.cryptocompare.com  and save it in KEY 
- It is possible to have some VIP coins that when they start to grow, they will be listed with different color to get your attention.

## How program works ##

The program monitors the price of different coins every one minute and act  based on following rules:

- When a coin grows at least 2.5 percent, It shows in the list of  growing coins some useful information.
- It plays a sound to get your attention
- When the price of coin decreases it remotes it from the list and also resets the coin info if it decreases to lower than minimum.
- It saves how many times the coin grows more that 2.5 and 10 percent in a sqlite database to give you some statistical data about it.

