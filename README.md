# Online Crypto Analyzer 
I write this program to get rich soon. but after loosing $500 in the market I find that it needs more time and energy and skill to monitor crypto market and earn money. 
Crypto market is very fluctuating and expert guys can earn money from it.
Anyway I decided to develop this program to monitor the price of some  cryptoz and when those cryptoz start losing value against BTC, I buy them and after earning 5 percent or more, I sell them. 
I get a key from www.cryptocompare.com and use the API of them to get the price of those coins.

To have flexibility to add or remove coins, I use a config.ini file which has 3 keys:

Config.ini sample contents:

[GENERAL]

COINS=WING,AVAX,UNFI,BTC,ETH,AXS,ALICE,BLZ,GRT,TRB,BABYDOGE,SHIB,SOL,CHZ

KEY=get your key from www.cryptocompare.com and put here

VIP=WING,AVAX,UNFI,SOL,CHZ

# Description of Config.ini keys
- add your favorite coins to COINS key like above ( all upper case)
- get an API  key from www.cryptocompare.com  and save it in KEY 
- It is possible to have some VIP coins that when they start to grow, they will be listed with different color (YELLOW) to get your attention.

## How program works ##

The program monitors the price of different coins every one minute and act  based on following rules:

- When a coin loses value at least 3 percent against Bitcoin price, It shows in the list of coins.
- It plays a sound to get your attention


> Visual Studio 15 is used to develop this program.
>
> Curl command line is used to send and receive REST API to cryptocompare website.
> 
> Libsdl is used to play audio 
>
> After compiling, copy necessary DLL files to the Debug or Release folders. 

