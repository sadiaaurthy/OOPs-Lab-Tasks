#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <ctime>
#include <cstdlib>
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
