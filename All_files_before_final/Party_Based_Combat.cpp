#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <ctime>
#include <cstdlib>

// =========================
// Enums / Small Types
// =========================
enum class StatType { MaxHP, Attack, Defense, Speed, Accuracy, Evasion, Crit };
enum class DamageType { Physical, Magical, True };
enum class TickTiming { TurnStart, TurnEnd };
enum class BuffType { AttackBuff, DefenseBuff, Nerf };

// =========================
// Resource Class 
// =========================
class Resource { 
private:
    int current;
    int max;
public:
    // Constructors
    Resource() : current(0), max(0) {}
    Resource(int cur, int mx) : current(cur), max(mx) {}

    // Getters
    int getCurrent() const { return current; }
    int getMax() const { return max; }

    // Setters with validation
    void setCurrent(int value) { 
        current = std::clamp(value, 0, max); 
    }
    
    void setMax(int value) { 
        max = std::max(0, value);
        if (current > max) current = max;
    }

    // Utility methods
    void restore(int amount) {
        current = std::min(max, current + amount); // curr is capped at max
    }

    void consume(int amount) {
        current = std::max(0, current - amount);
    }

    bool isEmpty() const { return current == 0; }
    bool isFull() const { return current == max; }
    
    float getPercentage() const { 
        return max > 0 ? static_cast<float>(current) / max : 0.0f; 
    }
};

// =========================
// StatBlock Class 
// =========================
class StatBlock {
private:
    int maxHP;
    int attack;
    int defense;
    int speed;
    int accuracy; //chance to hit
    int evasion;  //chance to dodge
    int crit;     //chance to deal critical damage

public:
    // Default constructor
    StatBlock() 
        : maxHP(100), attack(10), defense(5), speed(10),
          accuracy(100), evasion(0), crit(0) {}

    // Parameterized constructor with initialization list
    StatBlock(int hp, int atk, int def, int spd, int acc, int eva, int crt)
        : maxHP(hp), attack(atk), defense(def), speed(spd),
          accuracy(acc), evasion(eva), crit(crt) {}

    // Getters (const for read-only access)
    int getMaxHP() const { return maxHP; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getSpeed() const { return speed; }
    int getAccuracy() const { return accuracy; }
    int getEvasion() const { return evasion; }
    int getCrit() const { return crit; }

    // Setters with validation (encapsulation)
    void setMaxHP(int value) { maxHP = std::max(1, value); }
    void setAttack(int value) { attack = std::max(0, value); }
    void setDefense(int value) { defense = std::max(0, value); }
    void setSpeed(int value) { speed = std::max(1, value); }
    void setAccuracy(int value) { accuracy = std::clamp(value, 0, 100); }
    void setEvasion(int value) { evasion = std::clamp(value, 0, 100); }
    void setCrit(int value) { crit = std::clamp(value, 0, 100); } // restricts a value to lie within a specified range defined by min & max bound

    // Dynamic stat access by type (abstraction: hide internal details (MaxHP, Attack..., Crit),expose only what's needed (getStat(StatType::Attack)))
    int getStat(StatType type) const {
        switch (type) {
            case StatType::MaxHP: return maxHP;
            case StatType::Attack: return attack;
            case StatType::Defense: return defense;
            case StatType::Speed: return speed;
            case StatType::Accuracy: return accuracy;
            case StatType::Evasion: return evasion;
            case StatType::Crit: return crit;
            default: return 0;
        }
    }

    void setStat(StatType type, int value) {
        switch (type) {
            case StatType::MaxHP: setMaxHP(value); break;
            case StatType::Attack: setAttack(value); break;
            case StatType::Defense: setDefense(value); break;
            case StatType::Speed: setSpeed(value); break;
            case StatType::Accuracy: setAccuracy(value); break;
            case StatType::Evasion: setEvasion(value); break;
            case StatType::Crit: setCrit(value); break;
        }
    }

    // Utility methods
    void increaseStat(StatType type, int amount) {
        setStat(type, getStat(type) + amount);
    }

    void decreaseStat(StatType type, int amount) {
        increaseStat(type, -amount);
    }
};

// =========================
// StatModifier Class 
// =========================
class StatModifier {
private:
    StatType type;
    int flat; // + or - fixed amount (additive change)
    float mult; // percentage/scaling by multiplication

public:
    // Constructors
    StatModifier() : type(StatType::Attack), flat(0), mult(1.0f) {}
    
    StatModifier(StatType t, int f, float m) 
        : type(t), flat(f), mult(m) {}

    // Getters
    StatType getType() const { return type; }
    int getFlat() const { return flat; }
    float getMult() const { return mult; }

    // Setters
    void setType(StatType t) { type = t; }
    void setFlat(int value) { flat = value; }
    void setMult(float value) { mult = std::max(0.0f, value); }

    // Apply modifier to a value (abstraction)
    int applyToValue(int baseValue) const {
        return static_cast<int>((baseValue + flat) * mult);
    }

    // Utility
    bool isBuffing() const { return flat > 0 || mult > 1.0f; } // define what kind of modifier it is--> incr
    bool isDebuffing() const { return flat < 0 || mult < 1.0f; } // define what kind of modifier it is--> decr
};

// =========================
// BattleEvent Class 
// =========================
class BattleEvent {
private:
    std::string text;

public:
    // Constructor
    BattleEvent() : text("") {}
    explicit BattleEvent(std::string msg) : text(std::move(msg)) {} // move func is static cast converts it's arg into rvalue ref
    /*'explicit' prevents automatic(implicit) type conversion when calling a constructor
      BattleEvent e = "Hero attacks!" (This String'll be automatic converted to BattleEvent 
      it'll be a compile-time error) but BattleEvent e("Hero attacks!"); clear intention */

    // Getters
    const std::string& getText() const { return text; }

    // Setters
    void setText(const std::string& msg) { text = msg; }
};

// Forward declarations
class Battle;
class Character;
class Party;
class PlayerParty;
class OpponentParty;   
class Action;
class Skill;
class Effect;
class StatusEffect;
class Buff;
class TargetingRule;
class BattleRules;
class TurnManager;
class AIController;
class BattleLog;

// Convenience typedefs
using CharPtr = std::shared_ptr<Character>; // Stores the address(in heap mem) of an obj of type 'Character'
using PartyPtr = std::shared_ptr<Party>; // Holds the address of a derived obj
using ActPtr = std::shared_ptr<Action>;  // but shared_ptr<> is a smart ptr that deletes obj automatically when nobody is using it also keep tracks of how many owners exists
using SkillPtr = std::shared_ptr<Skill>;
using EffPtr = std::shared_ptr<Effect>;
using StatusPtr = std::shared_ptr<StatusEffect>;
using BuffPtr = std::shared_ptr<Buff>;
// std::vector<CharPtr> members; This vector can hold PlayerCharacter, OpponentCharacter or Any future character type, all treated uniformly as 'Character'
// unique_ptr --> single owner
// shared_ptr --> multiple owners
// A Character is used by Party, Battle, Action, AIController, TargetinRule
// A base-class ptr can point to a derived-class obj, calles to virtual funcs are resolved at runtime

// =========================
// BattleLog
// =========================
class BattleLog {
private:
    std::vector<BattleEvent> events;

public:
    // Add event
    void add(const std::string& msg) { 
        events.push_back(BattleEvent(msg)); 
    }
    
    // Display all events
    void dumpToStdout() const {
        for (const auto& e : events) 
            std::cout << e.getText() << "\n";
    }
    
    // Get all events
    const std::vector<BattleEvent>& getEvents() const { 
        return events; 
    }

    // Clear events
    void clear() {
        events.clear();
    }

    // Get event count
    size_t getEventCount() const {
        return events.size();
    }
};

// =========================
// Buff System
// =========================
class Buff {
private:
    std::string name;
    BuffType type;
    int value;
    int duration;

public:
    Buff(std::string buffName, BuffType buffType, int buffValue, int buffDuration)
        : name(std::move(buffName)), type(buffType), value(buffValue), duration(buffDuration) {}

    // Getters
    const std::string& getName() const { return name; }
    BuffType getType() const { return type; }
    int getValue() const { return value; }
    int getDuration() const { return duration; }
    
    // Setters
    void setDuration(int dur) { duration = std::max(0, dur); }
    void setValue(int val) { value = val; }
    
    // Duration management
    void decrementDuration() { 
        if (duration > 0) duration--; 
    }
    
    bool isExpired() const { 
        return duration <= 0; 
    }

    // Utility
    void reset(int newDuration) {
        duration = newDuration;
    }
};

// ============================================
// TargetingRule (Abstract Base - Polymorphism)
// ============================================
class TargetingRule {    // TargetingRule obj can't be created
public:
    virtual ~TargetingRule() = default;
    virtual std::vector<CharPtr> validTargets(const CharPtr& actor, Battle& battle) const = 0;
    
    // Additional utility (can be overridden)
    virtual bool canTargetAllies() const { return false; }
    virtual bool canTargetEnemies() const { return true; }
};
// Different skill diff targeting logic
// Fireball -> single enemy, Buff -> ally, Explosion -> all enemies.....
class SingleEnemyRule : public TargetingRule {
public:
    std::vector<CharPtr> validTargets(const CharPtr& actor, Battle& battle) const override;
    bool canTargetEnemies() const override { return true; }
};

class SelfRule : public TargetingRule {
public:
    std::vector<CharPtr> validTargets(const CharPtr& actor, Battle& battle) const override;
    bool canTargetAllies() const override { return true; }
};

class AllEnemiesRule : public TargetingRule {
public:
    std::vector<CharPtr> validTargets(const CharPtr& actor, Battle& battle) const override;
    bool canTargetEnemies() const override { return true; }
};

class AllyRule : public TargetingRule {
public:
    std::vector<CharPtr> validTargets(const CharPtr& actor, Battle& battle) const override;
    bool canTargetAllies() const override { return true; }
};

// ======================================
// Effects (Abstract Base - Polymorphism)
// ======================================
class Effect {
public:
    virtual ~Effect() = default;
    virtual void apply(Battle& battle, const CharPtr& actor, const CharPtr& target) = 0;
    // Every Effect must know how to 'apply()' itself --> POLYMORPHIC CONTRACT
    // What role 'apply()' will play is defined outside each classes but declaration is inside classes
    
    // Optional: Get effect description
    virtual std::string getDescription() const { return "Effect"; }
};

class DamageEffect : public Effect {
private:
    int baseDamage;
    DamageType dmgType;

public:
    DamageEffect(int base, DamageType type) 
        : baseDamage(base), dmgType(type) {}
    
    void apply(Battle& battle, const CharPtr& actor, const CharPtr& target) override;
    
    std::string getDescription() const override {
        return "Deals " + std::to_string(baseDamage) + " damage";
    }
    
    // Getters
    int getBaseDamage() const { return baseDamage; }
    DamageType getDamageType() const { return dmgType; }
};

class HealEffect : public Effect {
private:
    int amount;

public:
    explicit HealEffect(int healAmount) : amount(healAmount) {}
    void apply(Battle& battle, const CharPtr& actor, const CharPtr& target) override;
    
    std::string getDescription() const override {
        return "Heals " + std::to_string(amount) + " HP";
    }
    
    int getAmount() const { return amount; }
};

class ApplyStatusEffect : public Effect {
private:
    StatusPtr status;

public:
    explicit ApplyStatusEffect(StatusPtr statusEffect) 
        : status(std::move(statusEffect)) {}
    
    void apply(Battle& battle, const CharPtr& actor, const CharPtr& target) override;
    // In the runtime the overridden version is executed, not the base one
    
    std::string getDescription() const override {
        return "Applies status effect";
    }
};

class ApplyBuffEffect : public Effect {
private:
    BuffPtr buff;

public:
    explicit ApplyBuffEffect(BuffPtr buffEffect) 
        : buff(std::move(buffEffect)) {}
    
    void apply(Battle& battle, const CharPtr& actor, const CharPtr& target) override;
    
    std::string getDescription() const override {
        return "Applies buff";
    }
};

// =========================
// Status Effects (Abstract Base - Polymorphism)
// =========================
class StatusEffect {
private:
    std::string name;
    int duration;
    TickTiming timing;

public:
    StatusEffect(std::string statusName, int statusDuration, TickTiming statusTiming)
        : name(std::move(statusName)), duration(statusDuration), timing(statusTiming) {}

    virtual ~StatusEffect() = default;

    // Virtual methods (polymorphism)
    virtual void onApply(Battle& battle, const CharPtr& target) { (void)battle; (void)target; } // Casting to 'void' making the variables intentionally unused
    virtual void onExpire(Battle& battle, const CharPtr& target) { (void)battle; (void)target; }
    virtual void onTick(Battle& battle, const CharPtr& target) { (void)battle; (void)target; }
    virtual bool blocksAction() const { return false; }
    virtual std::vector<StatModifier> modifiers() const { return {}; }

    // Getters
    const std::string& getName() const { return name; }
    TickTiming getTiming() const { return timing; }
    int getDuration() const { return duration; }
    
    // Setters
    void setDuration(int dur) { duration = std::max(0, dur); }
    
    // Duration management
    void decrement() { 
        duration = std::max(0, duration - 1); 
    }
    
    bool expired() const { 
        return duration <= 0; 
    }

    // Utility
    virtual bool isDebuff() const { return true; }
    virtual bool isBuff() const { return false; }
};

class PoisonStatus : public StatusEffect {
private:
    int perTick;

public:
    PoisonStatus(int duration, int damage)
        : StatusEffect("Poison", duration, TickTiming::TurnEnd), perTick(damage) {}

    void onTick(Battle& battle, const CharPtr& target) override;
    
    bool isDebuff() const override { return true; }
    
    int getDamagePerTick() const { return perTick; }
};

class StunStatus : public StatusEffect {
public:
    explicit StunStatus(int duration)
        : StatusEffect("Stun", duration, TickTiming::TurnStart) {}

    bool blocksAction() const override { return true; }
    bool isDebuff() const override { return true; }
};

class OverencumberedStatus : public StatusEffect { // warrior carrying heavy armor + loot becomes overencumbered, so, they move slower
private:
    int speedReduction; // How much speed should be reduced during overencumbered

public:
    OverencumberedStatus(int duration, int speedPenalty)
        : StatusEffect("Overencumbered", duration, TickTiming::TurnStart), 
          speedReduction(speedPenalty) {}

    std::vector<StatModifier> modifiers() const override {
        return {StatModifier(StatType::Speed, -speedReduction, 1.0f)};
    }
    
    bool isDebuff() const override { return true; }
    
    int getSpeedReduction() const { return speedReduction; }
};

// =========================
// Skill / Ability (Abstract Base - Polymorphism)
// =========================
class Skill {                 // Abs. base that can be extended by specific skills like Offensive skills(damage-dealing skills...), Defensive skills(heals, buffs...)
private:
    std::string name;         // e.g., Fireball, Heal...
    int cost;                 // e.g., mana or energy cost
    int cooldown;             // e.g., How many turns before the skill can be used again
    int currentCooldown;  
    std::shared_ptr<TargetingRule> rule; // e.g., who can be targeted (single enemy, self, all enemies...) by the skill

protected:
    void resetCooldown() { currentCooldown = cooldown; }
    void tickCooldown() {      //over the time each turn decreases the currentCooldown
        if (currentCooldown > 0) currentCooldown--; 
    }

public:
    Skill(std::string skillName, int manaCost, int skillCooldown, 
          std::shared_ptr<TargetingRule> targetRule)
        : name(std::move(skillName)), cost(manaCost), cooldown(skillCooldown),
          currentCooldown(0), rule(std::move(targetRule)) {}

    virtual ~Skill() = default;

    // Getters
    const std::string& getName() const { return name; }
    int getCost() const { return cost; }
    int getCooldown() const { return cooldown; }
    int getCurrentCooldown() const { return currentCooldown; }
    
    // Setters
    void setCost(int newCost) { cost = std::max(0, newCost); }
    void setCooldown(int newCooldown) { cooldown = std::max(0, newCooldown); }
    
    // Virtual methods (polymorphism)
    virtual bool canUse(const CharPtr& actor) const;
    virtual void use(Battle& battle, const CharPtr& actor, 
                     const std::vector<CharPtr>& targets) = 0;

    std::vector<CharPtr> validTargets(const CharPtr& actor, Battle& battle) const {
        return rule ? rule->validTargets(actor, battle) : std::vector<CharPtr>{};
    }
    
    // Cooldown management
    void decrementCooldown() { tickCooldown(); }
    void putOnCooldown() { resetCooldown(); }
    void resetCurrentCooldown() { currentCooldown = 0; }
    
    // Utility
    bool isReady() const { return currentCooldown == 0; }
    virtual std::string getDescription() const { return "Skill"; }
};

class CompositeSkill : public Skill {  // Multiple eff in 1 action: damage & heal or buffs the player while applying a debuff to the enemy
private:
    std::vector<EffPtr> effects;

public:
    CompositeSkill(std::string name, int cost, int cooldown, 
                   std::shared_ptr<TargetingRule> rule)
        : Skill(std::move(name), cost, cooldown, std::move(rule)) {}

    void addEffect(EffPtr eff) { 
        effects.push_back(std::move(eff)); 
    }

    void use(Battle& battle, const CharPtr& actor, 
             const std::vector<CharPtr>& targets) override;
    
    std::string getDescription() const override {
        std::string desc = getName() + ": ";
        for (const auto& eff : effects) {
            if (eff) desc += eff->getDescription() + " ";
        }
        return desc;
    }
    
    size_t getEffectCount() const { return effects.size(); }
};

// =========================
// Character (Abstract Base Class - Polymorphism)
// =========================
class Character : public std::enable_shared_from_this<Character> {
private:
    std::string name;
    int level;
    StatBlock baseStats;
    int hp;
    Resource mana;
    int weight;
    int maxWeight;
    bool defending;
    
    std::vector<SkillPtr> skills;
    std::vector<StatusPtr> statuses;
    std::vector<BuffPtr> buffs;
    
    Party* ownerParty;  // Composition - Character HAS-A Party relationship

public:
    Character(std::string charName, int charLevel, StatBlock stats, 
              int startWeight = 0, int weightLimit = 100)
        : name(std::move(charName)), level(charLevel), baseStats(stats),
          hp(stats.getMaxHP()), mana(0, 100), weight(startWeight), 
          maxWeight(weightLimit), defending(false), ownerParty(nullptr) {}

    virtual ~Character() = default;

    // Getters (const for read-only access - encapsulation)
    const std::string& getName() const { return name; }
    int getHP() const { return hp; }
    int getMaxHP() const { return getFinalStat(StatType::MaxHP); }
    int getLevel() const { return level; }
    int getWeight() const { return weight; }
    int getMaxWeight() const { return maxWeight; }
    int getMana() const { return mana.getCurrent(); }
    int getMaxMana() const { return mana.getMax(); }
    bool isDefending() const { return defending; }
    Party* getOwnerParty() const { return ownerParty; }
    const std::vector<SkillPtr>& getSkills() const { return skills; }
    const StatBlock& getBaseStats() const { return baseStats; }
    
    bool isAlive() const { return hp > 0; }
    bool isOverencumbered() const { return weight > maxWeight; }

    // Setters (validation - encapsulation)
    void setOwnerParty(Party* party) { ownerParty = party; }
    void setDefending(bool def) { defending = def; }
    void setLevel(int lvl) { level = std::max(1, lvl); }
    
    void addWeight(int amount) { 
        weight += amount;
        checkOverencumbered();
    }
    
    void removeWeight(int amount) { 
        weight = std::max(0, weight - amount);
    }

    // Core operations (abstraction)
    void takeDamage(int amount) { 
        hp = std::max(0, hp - std::max(0, amount)); 
    }
    
    void heal(int amount) { 
        hp = std::min(getMaxHP(), hp + std::max(0, amount)); 
    }
    
    void consumeMana(int amount) {
        mana.consume(amount);
    }
    
    void restoreMana(int amount) {
        mana.restore(amount);
    }

    void addSkill(SkillPtr s) { 
        skills.push_back(std::move(s)); 
    }

    void removeSkill(const SkillPtr& s) {
        skills.erase(std::remove(skills.begin(), skills.end(), s), skills.end());
    }

    void applyStatus(Battle& battle, StatusPtr status);
    void applyBuff(Battle& battle, BuffPtr buff);
    void tickStatuses(Battle& battle, TickTiming when);
    void tickBuffs(Battle& battle);
    void tickCooldowns();
    bool canAct() const;
    void checkOverencumbered();

    // Stat computation (base + buffs + status mods) - abstraction
    int getFinalStat(StatType type) const;
    
    // Calculate modified attack with buffs
    int getModifiedAttack() const;
    
    // Calculate modified defense with buffs
    int getModifiedDefense() const;
    
    // Apply nerf effect (percentage health reduction)
    void applyNerf(int percentage, Battle& battle);

    // Pure virtual - different behavior for player/opponent characters (polymorphism)
    virtual void onTurnStart(Battle& battle) = 0;
    virtual void onTurnEnd(Battle& battle) = 0;
    
    // Additional virtual methods
    virtual bool isPlayerControlled() const = 0;
};

// =========================
// Party (Abstract Base Class - Polymorphism)
// =========================
class Party {
private:
    std::string partyName;
    int partyId;
    std::vector<CharPtr> members;
    int maxMembers;

public:
    Party(std::string name, int id, int maxSize)
        : partyName(std::move(name)), partyId(id), maxMembers(maxSize) {}

    virtual ~Party() = default;

    // Getters (encapsulation)
    const std::string& getPartyName() const { return partyName; }
    int getPartyId() const { return partyId; }
    int getMaxMembers() const { return maxMembers; }
    const std::vector<CharPtr>& getMembers() const { return members; }
    
    std::vector<CharPtr> getAliveMembers() const {
        std::vector<CharPtr> alive;
        for (const auto& m : members) {
            if (m->isAlive()) alive.push_back(m);
        }
        return alive;
    }

    int getAliveCount() const {
        return static_cast<int>(getAliveMembers().size());
    }

    bool isDefeated() const {
        return getAliveCount() == 0;
    }

    bool isFull() const {
        return static_cast<int>(members.size()) >= maxMembers;
    }

    // Setters
    void setPartyName(const std::string& name) { partyName = name; }
    void setMaxMembers(int max) { maxMembers = std::max(1, max); }

    // Add member with validation (encapsulation)
    virtual bool addMember(const CharPtr& character) {
        if (isFull()) {
            std::cout << "Party is full!\n";
            return false;
        }
        
        members.push_back(character);
        character->setOwnerParty(this);
        return true;
    }

    // Remove member
    void removeMember(const CharPtr& character) {
        members.erase(
            std::remove(members.begin(), members.end(), character),
            members.end()
        );
    }

    // Pure virtual - different turn handling for player/opponent parties (polymorphism)
    virtual void executeTurn(Battle& battle) = 0;
    virtual void onTurnStart(Battle& battle) = 0;
    virtual void onTurnEnd(Battle& battle) = 0;
    
    // Additional virtual methods
    virtual bool isPlayerParty() const = 0;
};

// =========================
// PlayerParty - Inheritance from Party
// =========================
class PlayerParty : public Party {
private:
    int totalWeight;
    int partyWeightLimit;

public:
    PlayerParty(std::string name, int id, int weightLimit = 400)
        : Party(std::move(name), id, 4), // Max 4 players
          totalWeight(0), partyWeightLimit(weightLimit) {}

    // Getters
    int getTotalWeight() const { return totalWeight; }
    int getPartyWeightLimit() const { return partyWeightLimit; }
    
    // Setters
    void setPartyWeightLimit(int limit) { partyWeightLimit = std::max(0, limit); }
    
    bool isPartyOverencumbered() const {
        return totalWeight > partyWeightLimit;
    }

    bool addMember(const CharPtr& character) override {
        if (Party::addMember(character)) {
            totalWeight += character->getWeight();
            
            if (isPartyOverencumbered()) {
                std::cout << "Warning: Party is overencumbered!\n";
            }
            return true;
        }
        return false;
    }

    void updateTotalWeight() {
        totalWeight = 0;
        for (const auto& member : getMembers()) {
            totalWeight += member->getWeight();
        }
    }

    // Override virtual methods (polymorphism)
    void executeTurn(Battle& battle) override;
    void onTurnStart(Battle& battle) override;
    void onTurnEnd(Battle& battle) override;
    bool isPlayerParty() const override { return true; }
};

// =========================
// OpponentParty - Inheritance from Party
// =========================
class OpponentParty : public Party {
private:
    std::shared_ptr<AIController> aiController;

public:
    OpponentParty(std::string name, int id, int maxSize = 6)
        : Party(std::move(name), id, maxSize) {}

    void setAI(std::shared_ptr<AIController> ai) {
        aiController = std::move(ai);
    }

    std::shared_ptr<AIController> getAI() const {
        return aiController;
    }

    // Override virtual methods (polymorphism)
    void executeTurn(Battle& battle) override;
    void onTurnStart(Battle& battle) override;
    void onTurnEnd(Battle& battle) override;
    bool isPlayerParty() const override { return false; }
};

// =========================
// PlayerCharacter - Inheritance from Character
// =========================
class PlayerCharacter : public Character {
public:
    PlayerCharacter(std::string name, int level, StatBlock stats, 
                    int startWeight = 0, int weightLimit = 100)
        : Character(std::move(name), level, stats, startWeight, weightLimit) {}

    // Override virtual methods (polymorphism)
    void onTurnStart(Battle& battle) override;
    void onTurnEnd(Battle& battle) override;
    bool isPlayerControlled() const override { return true; }
};

// =========================
// OpponentCharacter - Inheritance from Character
// =========================
class OpponentCharacter : public Character {
private:
    std::shared_ptr<AIController> individualAI;

public:
    OpponentCharacter(std::string name, int level, StatBlock stats)
        : Character(std::move(name), level, stats, 0, 200) {}

    void setAI(std::shared_ptr<AIController> ai) {
        individualAI = std::move(ai);
    }

    std::shared_ptr<AIController> getAI() const {
        return individualAI;
    }

    // Override virtual methods (polymorphism)
    void onTurnStart(Battle& battle) override;
    void onTurnEnd(Battle& battle) override;
    bool isPlayerControlled() const override { return false; }
};

// =========================
// Action (Abstract Base - Polymorphism)
// =========================
class Action {
protected:
    CharPtr actor;
    int priority;

public:
    Action(CharPtr actionActor, int actionPriority = 0) 
        : actor(std::move(actionActor)), priority(actionPriority) {}
    
    virtual ~Action() = default;

    // Getters
    int getPriority() const { return priority; }
    const CharPtr& getActor() const { return actor; }

    // Setters
    void setPriority(int p) { priority = p; }

    // Pure virtual methods (polymorphism)
    virtual bool validate(Battle& battle) const = 0;
    virtual void execute(Battle& battle) = 0;
    virtual std::string getActionName() const = 0;
};

class BasicAttackAction : public Action {
private:
    CharPtr target;

public:
    BasicAttackAction(CharPtr attacker, CharPtr defender)
        : Action(std::move(attacker), 0), target(std::move(defender)) {}

    bool validate(Battle& battle) const override;
    void execute(Battle& battle) override;
    std::string getActionName() const override { return "Basic Attack"; }
    
    const CharPtr& getTarget() const { return target; }
};

class SkillAction : public Action {
private:
    SkillPtr skill;
    std::vector<CharPtr> targets;

public:
    SkillAction(CharPtr user, SkillPtr ability, std::vector<CharPtr> skillTargets)
        : Action(std::move(user), 0), skill(std::move(ability)), 
          targets(std::move(skillTargets)) {}

    bool validate(Battle& battle) const override;
    void execute(Battle& battle) override;
    std::string getActionName() const override { return "Skill: " + skill->getName(); }
    
    const SkillPtr& getSkill() const { return skill; }
    const std::vector<CharPtr>& getTargets() const { return targets; }
};

class DefendAction : public Action {
public:
    explicit DefendAction(CharPtr defender) 
        : Action(std::move(defender), 1) {}
    
    bool validate(Battle& battle) const override { 
        (void)battle; 
        return true; 
    }
    void execute(Battle& battle) override;
    std::string getActionName() const override { return "Defend"; }
};

// =========================
// BattleRules
// =========================
class BattleRules {
private:
    int minHitChance;
    int maxHitChance;

public:
    BattleRules() : minHitChance(5), maxHitChance(95) {}
    
    virtual ~BattleRules() = default;

    // Getters
    int getMinHitChance() const { return minHitChance; }
    int getMaxHitChance() const { return maxHitChance; }

    // Setters
    void setMinHitChance(int chance) { minHitChance = std::clamp(chance, 0, 100); }
    void setMaxHitChance(int chance) { maxHitChance = std::clamp(chance, 0, 100); }

    virtual bool rollHit(const CharPtr& actor, const CharPtr& target) const {
        int acc = actor->getFinalStat(StatType::Accuracy);
        int eva = target->getFinalStat(StatType::Evasion);
        int chance = std::clamp(acc - eva, minHitChance, maxHitChance);
        int r = rand() % 100;
        return r < chance;
    }

    virtual int computeDamage(const CharPtr& actor, const CharPtr& target, 
                              int base, DamageType type) const {
        (void)type;
        int atk = actor->getModifiedAttack();
        int def = target->getModifiedDefense();
        int raw = base + atk - def;

        if (target->isDefending()) {
            raw = raw / 2;
        }
        
        return std::max(0, raw);
    }

    virtual bool rollCrit(const CharPtr& actor) const {
        int critChance = actor->getFinalStat(StatType::Crit);
        int r = rand() % 100;
        return r < critChance;
    }
};

// =========================
// TurnManager
// =========================
class TurnManager {
private:
    std::vector<PartyPtr> partyQueue;
    size_t currentPartyIndex;
    int round;

public:
    TurnManager() : currentPartyIndex(0), round(0) {}

    // Getters
    int getRound() const { return round; }
    size_t getCurrentPartyIndex() const { return currentPartyIndex; }
    const std::vector<PartyPtr>& getPartyQueue() const { return partyQueue; }

    void buildOrder(const std::vector<PartyPtr>& parties) {
        partyQueue.clear();
        
        for (const auto& party : parties) {
            if (!party->isDefeated()) {
                partyQueue.push_back(party);
            }
        }

        // Sort by average speed of alive members (abstraction)
        std::sort(partyQueue.begin(), partyQueue.end(), 
                  [](const PartyPtr& a, const PartyPtr& b) {
            int avgSpeedA = 0, avgSpeedB = 0;
            auto aliveA = a->getAliveMembers();
            auto aliveB = b->getAliveMembers();
            
            for (const auto& m : aliveA) {
                avgSpeedA += m->getFinalStat(StatType::Speed);
            }
            for (const auto& m : aliveB) {
                avgSpeedB += m->getFinalStat(StatType::Speed);
            }
            
            if (!aliveA.empty()) avgSpeedA /= static_cast<int>(aliveA.size());
            if (!aliveB.empty()) avgSpeedB /= static_cast<int>(aliveB.size());
            
            return avgSpeedA > avgSpeedB;
        });

        currentPartyIndex = 0;
        round++;
    }

    PartyPtr nextParty() {
        if (partyQueue.empty()) return nullptr;

        for (size_t tries = 0; tries < partyQueue.size(); tries++) {
            auto& party = partyQueue[currentPartyIndex];
            currentPartyIndex = (currentPartyIndex + 1) % partyQueue.size();
            
            if (!party->isDefeated()) {
                return party;
            }
        }
        
        return nullptr;
    }

    void resetRound() {
        round = 0;
    }
};

// =========================
// AIController (Abstract Base - Polymorphism)
// =========================
class AIController {
public:
    virtual ~AIController() = default;
    virtual ActPtr chooseAction(Battle& battle, const CharPtr& actor) = 0;
    virtual std::string getAIType() const = 0;
};

class RandomAI : public AIController {
public:
    ActPtr chooseAction(Battle& battle, const CharPtr& actor) override;
    std::string getAIType() const override { return "Random"; }
};

class AggressiveAI : public AIController {
public:
    ActPtr chooseAction(Battle& battle, const CharPtr& actor) override;
    std::string getAIType() const override { return "Aggressive"; }
};

// =========================
// Battle
// =========================
class Battle {
private:
    std::vector<PartyPtr> parties;
    TurnManager turnManager;
    std::shared_ptr<BattleRules> rules;
    std::shared_ptr<BattleLog> log;

public:
    Battle(std::shared_ptr<BattleRules> battleRules)
        : rules(std::move(battleRules)) {
        log = std::make_shared<BattleLog>();
    }

    // Getters
    std::shared_ptr<BattleLog> getLog() const { return log; }
    BattleRules& getRules() { return *rules; }
    const BattleRules& getRules() const { return *rules; }
    const std::vector<PartyPtr>& getParties() const { return parties; }
    const TurnManager& getTurnManager() const { return turnManager; }

    void addParty(const PartyPtr& party) { 
        parties.push_back(party); 
    }

    void removeParty(const PartyPtr& party) {
        parties.erase(std::remove(parties.begin(), parties.end(), party), parties.end());
    }

    std::vector<CharPtr> allCharacters() const {
        std::vector<CharPtr> all;
        for (const auto& party : parties) {
            for (const auto& member : party->getMembers()) {
                all.push_back(member);
            }
        }
        return all;
    }

    std::vector<CharPtr> enemiesOf(const CharPtr& actor) const {
        std::vector<CharPtr> enemies;
        Party* actorParty = actor->getOwnerParty();
        
        for (const auto& party : parties) {
            if (party.get() != actorParty) {
                for (const auto& member : party->getMembers()) {
                    if (member->isAlive()) {
                        enemies.push_back(member);
                    }
                }
            }
        }
        
        return enemies;
    }

    std::vector<CharPtr> alliesOf(const CharPtr& actor) const {
        std::vector<CharPtr> allies;
        Party* actorParty = actor->getOwnerParty();
        
        if (actorParty) {
            for (const auto& member : actorParty->getMembers()) {
                if (member->isAlive() && member != actor) {
                    allies.push_back(member);
                }
            }
        }
        
        return allies;
    }

    bool isOver() const {
        int aliveParties = 0;
        for (const auto& party : parties) {
            if (!party->isDefeated()) {
                aliveParties++;
            }
        }
        return aliveParties <= 1;
    }

    PartyPtr getWinner() const {
        for (const auto& party : parties) {
            if (!party->isDefeated()) {
                return party;
            }
        }
        return nullptr;
    }

    void start() {
        turnManager.buildOrder(parties);
        log->add("=== Battle Started! ===");
        
        for (const auto& party : parties) {
            log->add(party->getPartyName() + " enters the battle!");
        }
    }

    void executeRound() {
        if (isOver()) return;

        log->add("\n--- Round " + std::to_string(turnManager.getRound()) + " ---");

        // Rebuild party order at start of each round
        turnManager.buildOrder(parties);

        // Execute turn for each party
        for (size_t i = 0; i < parties.size() && !isOver(); i++) {
            PartyPtr party = turnManager.nextParty();
            
            if (party && !party->isDefeated()) {
                log->add("\n" + party->getPartyName() + "'s turn:");
                party->executeTurn(*this);
            }
        }
    }
};

// =========================
// Implementations
// =========================

// Character implementations
void Character::applyStatus(Battle& battle, StatusPtr status) {
    if (!status) return;
    
    statuses.push_back(status);
    battle.getLog()->add("  " + name + " gains status: " + status->getName());
    status->onApply(battle, shared_from_this());
}

void Character::applyBuff(Battle& battle, BuffPtr buff) {
    if (!buff) return;
    
    buffs.push_back(buff);
    battle.getLog()->add("  " + name + " gains buff: " + buff->getName());
}

void Character::tickStatuses(Battle& battle, TickTiming when) {
    for (auto& status : statuses) {
        if (status && status->getTiming() == when) {
            status->onTick(battle, shared_from_this());
            status->decrement();
            
            if (status->expired()) {
                battle.getLog()->add("  " + name + "'s " + status->getName() + " expired");
                status->onExpire(battle, shared_from_this());
            }
        }
    }

    statuses.erase(
        std::remove_if(statuses.begin(), statuses.end(),
            [](const StatusPtr& s) { return !s || s->expired(); }),
        statuses.end()
    );
}

void Character::tickBuffs(Battle& battle) {
    for (auto& buff : buffs) {
        if (buff) {
            buff->decrementDuration();
            
            if (buff->isExpired()) {
                battle.getLog()->add("  " + name + "'s " + buff->getName() + " expired");
            }
        }
    }

    buffs.erase(
        std::remove_if(buffs.begin(), buffs.end(),
            [](const BuffPtr& b) { return !b || b->isExpired(); }),
        buffs.end()
    );
}

void Character::tickCooldowns() {
    for (auto& skill : skills) {
        if (skill) {
            skill->decrementCooldown();
        }
    }
}

bool Character::canAct() const {
    for (const auto& status : statuses) {
        if (status && status->blocksAction()) {
            return false;
        }
    }
    return true;
}

void Character::checkOverencumbered() {
    // This is called automatically when weight changes
}

int Character::getFinalStat(StatType type) const {
    int base = baseStats.getStat(type);

    int flatSum = 0;
    float mult = 1.0f;

    // Apply status modifiers
    for (const auto& status : statuses) {
        if (!status) continue;
        
        for (const auto& mod : status->modifiers()) {
            if (mod.getType() == type) {
                flatSum += mod.getFlat();
                mult *= mod.getMult();
            }
        }
    }

    int val = static_cast<int>((base + flatSum) * mult);
    return std::max(0, val);
}

int Character::getModifiedAttack() const {
    int baseAttack = getFinalStat(StatType::Attack);
    
    // Apply attack buffs
    for (const auto& buff : buffs) {
        if (buff && buff->getType() == BuffType::AttackBuff) {
            baseAttack += buff->getValue();
        }
    }
    
    return baseAttack;
}

int Character::getModifiedDefense() const {
    int baseDefense = getFinalStat(StatType::Defense);
    
    // Apply defense buffs
    for (const auto& buff : buffs) {
        if (buff && buff->getType() == BuffType::DefenseBuff) {
            baseDefense += buff->getValue();
        }
    }
    
    return baseDefense;
}

void Character::applyNerf(int percentage, Battle& battle) {
    int damage = (hp * percentage) / 100;
    takeDamage(damage);
    
    battle.getLog()->add("  " + name + " is nerfed! Lost " + 
                        std::to_string(damage) + " HP (" + 
                        std::to_string(percentage) + "% of current health)");
}

// PlayerCharacter implementations
void PlayerCharacter::onTurnStart(Battle& battle) {
    tickStatuses(battle, TickTiming::TurnStart);
    tickCooldowns();
    
    if (isOverencumbered()) {
        battle.getLog()->add("  " + getName() + " is overencumbered! Movement hindered.");
        
        // Apply temporary speed reduction
        auto overencumberStatus = std::make_shared<OverencumberedStatus>(1, 5);
        applyStatus(battle, overencumberStatus);
    }
}

void PlayerCharacter::onTurnEnd(Battle& battle) {
    tickStatuses(battle, TickTiming::TurnEnd);
    tickBuffs(battle);
}

// OpponentCharacter implementations
void OpponentCharacter::onTurnStart(Battle& battle) {
    tickStatuses(battle, TickTiming::TurnStart);
    tickCooldowns();
}

void OpponentCharacter::onTurnEnd(Battle& battle) {
    tickStatuses(battle, TickTiming::TurnEnd);
    tickBuffs(battle);
}

// PlayerParty implementations
void PlayerParty::executeTurn(Battle& battle) {
    onTurnStart(battle);
    
    auto aliveMembers = getAliveMembers();
    
    // Each player in the party gets their own turn
    for (auto& member : aliveMembers) {
        if (!member->isAlive()) continue;
        
        member->onTurnStart(battle);
        
        if (!member->canAct()) {
            battle.getLog()->add("  " + member->getName() + " cannot act!");
            member->onTurnEnd(battle);
            continue;
        }

        member->setDefending(false);

        // Player input would go here
        // For now, use a simple AI for demonstration
        auto enemies = battle.enemiesOf(member);
        
        if (!enemies.empty()) {
            auto action = std::make_shared<BasicAttackAction>(member, enemies.front());
            
            if (action->validate(battle)) {
                action->execute(battle);
            }
        }

        member->onTurnEnd(battle);
    }
    
    onTurnEnd(battle);
}

void PlayerParty::onTurnStart(Battle& battle) {
    updateTotalWeight();
    
    if (isPartyOverencumbered()) {
        battle.getLog()->add("  " + getPartyName() + " is overencumbered! Total weight: " + 
                            std::to_string(getTotalWeight()) + "/" + 
                            std::to_string(getPartyWeightLimit()));
    }
}

void PlayerParty::onTurnEnd(Battle& battle) {
    (void)battle;
}

// OpponentParty implementations
void OpponentParty::executeTurn(Battle& battle) {
    onTurnStart(battle);
    
    auto aliveMembers = getAliveMembers();
    
    // Opponents act together as a coordinated unit
    for (auto& member : aliveMembers) {
        if (!member->isAlive()) continue;
        
        member->onTurnStart(battle);
        
        if (!member->canAct()) {
            battle.getLog()->add("  " + member->getName() + " cannot act!");
            member->onTurnEnd(battle);
            continue;
        }

        member->setDefending(false);

        // Use individual AI or party AI
        auto opponentChar = std::dynamic_pointer_cast<OpponentCharacter>(member);
        auto ai = opponentChar ? opponentChar->getAI() : aiController;
        
        if (ai) {
            auto action = ai->chooseAction(battle, member);
            
            if (action && action->validate(battle)) {
                action->execute(battle);
            }
        }

        member->onTurnEnd(battle);
    }
    
    onTurnEnd(battle);
}

void OpponentParty::onTurnStart(Battle& battle) {
    (void)battle;
}

void OpponentParty::onTurnEnd(Battle& battle) {
    (void)battle;
}

// Targeting rule implementations
std::vector<CharPtr> SingleEnemyRule::validTargets(const CharPtr& actor, Battle& battle) const {
    return battle.enemiesOf(actor);
}

std::vector<CharPtr> SelfRule::validTargets(const CharPtr& actor, Battle&) const {
    return {actor};
}

std::vector<CharPtr> AllEnemiesRule::validTargets(const CharPtr& actor, Battle& battle) const {
    return battle.enemiesOf(actor);
}

std::vector<CharPtr> AllyRule::validTargets(const CharPtr& actor, Battle& battle) const {
    return battle.alliesOf(actor);
}

// Skill implementations
bool Skill::canUse(const CharPtr& actor) const {
    return getCurrentCooldown() == 0 && actor->getMana() >= getCost();
}

void CompositeSkill::use(Battle& battle, const CharPtr& actor, 
                         const std::vector<CharPtr>& targets) {
    battle.getLog()->add("  " + actor->getName() + " uses " + getName() + "!");
    
    actor->consumeMana(getCost());
    putOnCooldown();
    
    for (const auto& target : targets) {
        if (!target || !target->isAlive()) continue;
        
        for (const auto& effect : effects) {
            effect->apply(battle, actor, target);
        }
    }
}

// Action implementations
bool BasicAttackAction::validate(Battle& battle) const {
    (void)battle;
    return actor && actor->isAlive() && target && target->isAlive();
}

void BasicAttackAction::execute(Battle& battle) {
    battle.getLog()->add("  " + actor->getName() + " attacks " + target->getName() + "!");

    if (!battle.getRules().rollHit(actor, target)) {
        battle.getLog()->add("    Miss!");
        return;
    }

    int dmg = battle.getRules().computeDamage(actor, target, 5, DamageType::Physical);
    target->takeDamage(dmg);
    battle.getLog()->add("    Deals " + std::to_string(dmg) + " damage. (" + 
                        target->getName() + ": " + std::to_string(target->getHP()) + 
                        "/" + std::to_string(target->getMaxHP()) + " HP)");

    if (!target->isAlive()) {
        battle.getLog()->add("    " + target->getName() + " is defeated!");
    }
}

bool SkillAction::validate(Battle& battle) const {
    (void)battle;
    
    if (!actor || !actor->isAlive() || !skill) return false;
    if (!skill->canUse(actor)) return false;
    
    for (const auto& target : targets) {
        if (target && target->isAlive()) return true;
    }
    
    return false;
}

void SkillAction::execute(Battle& battle) {
    skill->use(battle, actor, targets);
}

void DefendAction::execute(Battle& battle) {
    actor->setDefending(true);
    battle.getLog()->add("  " + actor->getName() + " takes a defensive stance!");
}

// Effect implementations
void DamageEffect::apply(Battle& battle, const CharPtr& actor, const CharPtr& target) {
    if (!actor || !target || !target->isAlive()) return;
    
    if (!battle.getRules().rollHit(actor, target)) {
        battle.getLog()->add("    Effect missed " + target->getName() + "!");
        return;
    }
    
    int dmg = battle.getRules().computeDamage(actor, target, baseDamage, dmgType);
    target->takeDamage(dmg);
    battle.getLog()->add("    " + target->getName() + " takes " + 
                        std::to_string(dmg) + " damage.");
}

void HealEffect::apply(Battle& battle, const CharPtr& actor, const CharPtr& target) {
    (void)actor;
    
    if (!target || !target->isAlive()) return;
    
    target->heal(amount);
    battle.getLog()->add("    " + target->getName() + " heals " + 
                        std::to_string(amount) + " HP.");
}

void ApplyStatusEffect::apply(Battle& battle, const CharPtr& actor, const CharPtr& target) {
    (void)actor;
    
    if (!target || !target->isAlive()) return;
    
    target->applyStatus(battle, status);
}

void ApplyBuffEffect::apply(Battle& battle, const CharPtr& actor, const CharPtr& target) {
    (void)actor;
    
    if (!target || !target->isAlive()) return;
    
    target->applyBuff(battle, buff);
}

// Status implementations
void PoisonStatus::onTick(Battle& battle, const CharPtr& target) {
    if (!target || !target->isAlive()) return;
    
    target->takeDamage(perTick);
    battle.getLog()->add("    " + target->getName() + " takes " + 
                        std::to_string(perTick) + " poison damage.");
}

// AI implementations
ActPtr RandomAI::chooseAction(Battle& battle, const CharPtr& actor) {
    auto enemies = battle.enemiesOf(actor);
    
    if (enemies.empty()) return nullptr;
    
    int idx = rand() % static_cast<int>(enemies.size());
    return std::make_shared<BasicAttackAction>(actor, enemies[idx]);
}

ActPtr AggressiveAI::chooseAction(Battle& battle, const CharPtr& actor) {
    auto enemies = battle.enemiesOf(actor);
    
    if (enemies.empty()) return nullptr;
    
    // Target enemy with lowest HP
    CharPtr target = enemies[0];
    
    for (const auto& enemy : enemies) {
        if (enemy->getHP() < target->getHP()) {
            target = enemy;
        }
    }
    
    return std::make_shared<BasicAttackAction>(actor, target);
}

// =========================
// Demo Main
// =========================
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    auto rules = std::make_shared<BattleRules>();
    Battle battle(rules);

    // Create player party
    auto playerParty = std::make_shared<PlayerParty>("Heroes", 0, 400);

    auto warrior = std::make_shared<PlayerCharacter>(
        "Warrior", 5, StatBlock(150, 18, 10, 12, 90, 5, 10), 50, 100
    );
    
    auto mage = std::make_shared<PlayerCharacter>(
        "Mage", 5, StatBlock(80, 25, 4, 8, 95, 10, 5), 30, 80
    );
    
    auto healer = std::make_shared<PlayerCharacter>(
        "Healer", 5, StatBlock(100, 10, 8, 10, 92, 8, 3), 40, 90
    );

    // Add skills to mage
    auto fireball = std::make_shared<CompositeSkill>(
        "Fireball", 20, 2, std::make_shared<SingleEnemyRule>()
    );
    fireball->addEffect(std::make_shared<DamageEffect>(30, DamageType::Magical));
    mage->addSkill(fireball);

    // Add skills to healer
    auto heal = std::make_shared<CompositeSkill>(
        "Heal", 15, 1, std::make_shared<SelfRule>()
    );
    heal->addEffect(std::make_shared<HealEffect>(40));
    healer->addSkill(heal);

    playerParty->addMember(warrior);
    playerParty->addMember(mage);
    playerParty->addMember(healer);

    // Create opponent party
    auto opponentParty = std::make_shared<OpponentParty>("Monsters", 1, 6);
    opponentParty->setAI(std::make_shared<AggressiveAI>());

    auto goblin1 = std::make_shared<OpponentCharacter>(
        "Goblin1", 3, StatBlock(70, 12, 5, 11, 85, 5, 0)
    );
    goblin1->setAI(std::make_shared<RandomAI>());

    auto goblin2 = std::make_shared<OpponentCharacter>(
        "Goblin2", 3, StatBlock(70, 12, 5, 11, 85, 5, 0)
    );
    goblin2->setAI(std::make_shared<RandomAI>());

    auto orc = std::make_shared<OpponentCharacter>(
        "Orc", 5, StatBlock(120, 16, 8, 8, 80, 2, 5)
    );
    orc->setAI(std::make_shared<AggressiveAI>());

    opponentParty->addMember(goblin1);
    opponentParty->addMember(goblin2);
    opponentParty->addMember(orc);

    // Add attack buff skill to warrior
    auto battleCry = std::make_shared<CompositeSkill>(
        "Battle Cry", 10, 3, std::make_shared<SelfRule>()
    );
    auto attackBuff = std::make_shared<Buff>("Attack Up", BuffType::AttackBuff, 5, 3);
    battleCry->addEffect(std::make_shared<ApplyBuffEffect>(attackBuff));
    warrior->addSkill(battleCry);

    battle.addParty(playerParty);
    battle.addParty(opponentParty);

    battle.start();

    // Execute combat rounds
    int maxRounds = 10;
    for (int i = 0; i < maxRounds && !battle.isOver(); i++) {
        battle.executeRound();
    }

    battle.getLog()->dumpToStdout();

    std::cout << "\n=== Battle Result ===\n";
    if (auto winner = battle.getWinner()) {
        std::cout << "Winner: " << winner->getPartyName() << "\n";
    } else {
        std::cout << "Draw!\n";
    }

    return 0;
}