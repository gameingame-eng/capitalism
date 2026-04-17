# Capitalism — Game Design Document

> A roguelike dungeon and outdoor adventure where everything has a price. The further you venture from town, the greater the danger — and the reward.

---

## Vision

You start in a small town with minimal gold and basic gear. The world extends outward in zones of increasing danger and wealth. Every run is a business decision: how far can you push before your supplies run out, your weapons degrade, and the debt collectors come knocking?

---

## World Structure

### Town (Spawn)
- Safe hub where all core services are located
- Inn for rest and saving (costs gold)
- Notice board with bounties and quests
- Bank to deposit gold so you don't lose it on death
- Shops: blacksmith, general store, alchemist, armorsmith

### Zones
| Zone | Area | Difficulty | Loot Quality |
|------|------|------------|--------------|
| 1 | Town outskirts / fields | Easy | Low |
| 2 | Forest / caves | Medium | Moderate |
| 3 | Ruins / abandoned settlements | Hard | High |
| 4 | Deep dungeon | Extreme | Rare / Unique |

- Enemy difficulty, loot quality, and merchant prices all scale with distance from town
- A visible **danger rating** ticks up the further you roam

---

## Core Economy

### Gold Flow
- The only source of income is selling loot back in town
- Everything else is a cost: food, water, ammo, rest, gear, entry fees, repairs
- Creates constant pressure to keep pushing further for better loot to cover costs

### Pricing by Distance
- Items have a base price that scales with the zone they were found in
- Zone multiplier: loot found in zone 3 sells for 3x but operating costs are also 3x
- Rare items only appear in deep zones with very high sell value

### Town Shops
- **Blacksmith** — weapons, repairs, upgrades using materials you bring back
- **General Store** — food, water, torches, basic ammo
- **Alchemist** — potions and antidotes
- **Armorsmith** — armor and shields
- Each shop has limited stock that restocks over time

### Supply & Demand
- Sell too many of the same item and the price drops
- Town has rotating "needs" — rare materials spike in price temporarily
- Seasonal and event-based demand shifts (e.g. town under threat raises weapon prices)

### Carrying Economy
- Inventory weight limit forces hard choices on what to bring back
- Heavier load = slower movement = more food and water consumed
- Bags and pack upgrades purchasable in town

### Upkeep Costs
- Weapons degrade and need repair (gold or materials)
- Armor has durability
- Ammo is finite and expensive deep in zones
- Forces active resource management rather than hoarding

---

## Survival Mechanics

- **Food and water meters** deplete over time
- Starving or dehydrated applies stat penalties
- Forces you to either return to town or find supplies from wilderness merchants
- Deep runs require careful pre-planning of supplies vs. carry weight

---

## Out-of-Town Economy

### Traveling Merchants
- Spawn randomly in wilderness zones
- Sell at 2x–3x markup vs town (captive market, no competition)
- Buy your loot at a discount — convenient but costly
- Rare merchant types in deep zones with unique stock

### Economy Tension Moments
- Deep in zone 4 with great loot but barely enough food to get back
- Deciding whether to sell now at a traveling merchant or risk carrying it to town for full price
- Watching your favorite weapon degrade at the worst possible time

---

## Debt & Credit

- Borrow gold from the bank with interest that compounds per run
- Miss payments and shops refuse to sell to you
- Deep debt triggers debt collector enemies that hunt you in the world
- Insurance system — pay upfront to recover gear on death
- Die with a loan outstanding and the next run starts in the red

---

## Stock Market

A broker NPC or notice board in town displays commodity prices that fluctuate based on your actions and world events.

### Tradeable Commodities
- Crafting materials: iron, steel, wood, leather
- Survival goods: food, water
- Weapon and armor types
- Magic components and rare drops from deep zones

### Price Drivers
- **Your actions** — selling 20 iron swords crashes the iron price
- **World events** — a goblin raid spikes weapon demand
- **Time / seasons** — food prices rise in dungeon "winter" cycles
- **Simulated traders** — ghost NPCs affect the market between your runs
- **Zone clears** — clearing a floor reduces its loot supply, temporarily raising prices

### Trading Mechanics
- Buy low, sell high on commodities between runs
- Futures contracts — lock in a price now, deliver goods later
- Short selling — bet a commodity price will drop, profit if it does

### Investing in Town Businesses
- Buy a stake in the blacksmith, general store, alchemist, etc.
- Earn a cut of their revenue passively each run
- Flooding their market with cheap goods hurts your own investment
- Businesses can go bankrupt if the economy collapses

### Market Manipulation
- Hoard a rare material then sell it all at once to crash the price
- Corner the market on a commodity and set your own price
- Bribe the broker for advance notice on price shifts
- Spread rumors via dialogue to inflate demand artificially

### Risk Events
- Market crashes — everything loses value suddenly
- Inflation — town prices rise across the board over time
- Monopoly NPCs that compete with you for market control
- Tax audits if you accumulate wealth too fast

### Balance Guardrails
- Investment returns are capped per run
- Heavy manipulation triggers regulatory NPCs or town suspicion
- Futures contracts expire worthless if you die before delivering
- Broker takes a cut on every transaction

### The Core Loop
1. Notice iron prices are high on the board
2. Gear up and head to the iron-rich zone 2 caves
3. Return loaded with ore and weapons
4. Sell at peak price, invest some profit into the blacksmith
5. Watch iron prices drop from the flood
6. Check what is rising next and plan your next run accordingly

---

## Enemy Concepts

- **Wildlife** — basic threats in outer zones
- **Bandits** — ambush and loot you if you are carrying a lot
- **Corporate enforcers** — deeper zones, protect high-value loot
- **Debt collectors** — spawn and hunt you when you miss loan payments
- **Landlord boss** — evicts you from a floor if you have not paid the entry fee
- **Bailiff enemies** — repossess your gear if you are in severe debt

---

## UI / Flavor

- Receipt-style transaction log for every purchase
- News ticker on the broker board explaining price shifts
- Commodity price graphs (green = rising, red = falling)
- Signs in the wilderness: *"Trespassing fine: 50g"*
- Town NPCs comment on how long you have been gone
- Satirical item flavor text: *"Sword — previously owned, minor blood stains, sold as-is"*
- Dungeon entrance is literally gated with a paid entry fee

---

## Economy Balance Philosophy

| Stage | Feel |
|-------|------|
| Early game | Barely breaking even, every purchase matters |
| Mid game | Starting to profit, upgrading gear, expanding range |
| Late game | Wealthy enough to take big risks, but costs scale too — always tense |

---

## Gambling

A casino or tavern in town lets you risk gold for a chance at big returns — or losing everything before your next run.

### Games
- **Dice** — simple high/low bets, fast and cheap
- **Cards** — higher skill ceiling, bigger swings
- **Slots** — loot-themed machine with rare jackpot items instead of gold
- **Fight betting** — wager on arena matches between captured monsters

### Mechanics
- House always has an edge, but odds improve with a "luck" stat you can invest in
- Hot streak system — win several in a row and the house cuts you off or raises limits
- Loan sharks in the casino offer instant gold at brutal interest rates
- Jackpot prizes can include rare gear, map locations, or stock tips

### Risk
- Gambling debt is tracked separately from bank debt — loan sharks send different, nastier collectors
- Casino can be rigged by certain NPCs you can bribe or expose
- Winning too much gets you banned until you pay a "membership fee"

---

## Tech Stack

- **Language:** C++
- **Engine:** raylib
- **Tileset:** Tiny 16 by Sharm (itch.io)

### Build Commands
- **Linux**: `g++ -std=c++17 ./src/main.cpp -o ./capitalism -I./src -I/include -L./lib/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11`
- **Windows**: `g++ -std=c++17 .\src\main.cpp -o .\capitalism.exe -I.\src -I.\include -L.\lib\win64 -lraylib -lopengl32 -lgdi32 -lwinmm`