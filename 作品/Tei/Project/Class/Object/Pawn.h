#pragma once
#include <array>
#include <list>
#include "../Input/INPUT_ID.h"
#include "Object.h"

namespace state
{
    struct Move;
    struct CheckKeyTrg;
    struct CheckKey;
    struct SetAnim;
    struct Stop;
    struct CheckRay;
    struct CheckG;
    struct ModuleNode;
    struct Fall;
    struct Jump;
    struct CheckAnim;
    struct Attack;
    struct Command;
    struct CheckAlive;
}

class Controller;
class Raycast;
class Bullet;

struct CommandData
{
    std::string name_;
    struct command
    {
        CMD_ID id;
        int time;
        bool required_;
    };
    std::list<command> command_;
    unsigned int allTime_ = 0;
};


// コントローラーの入力を受け付け、アニメーションが遷移するクラス
// @@アイテムとかこの先実装されなかったら何のために生まれたのかわからないクラス
class Pawn :
    public Object
{
public:
    Pawn(const Vector2Flt& pos, const Vector2& size,const Object_ID oID,int hp, TeamTag tag, InputType inputType = InputType::KEYBOARD);
    virtual ~Pawn() = default;
    virtual void Draw(const double& delta)override;
    virtual void AddDamage(int damage)override;
private:
  
protected:
    // アニメーション変更
    void SetAnimation(Char_Anim_ID id);

    std::map<Char_Anim_ID, std::function<bool(Object& obj)>> update_;

    Char_Anim_ID animID_;
    int animCnt_;
    int animLoopCnt_;

    float defJunpPower_;
    float yaddPower_;

    std::unique_ptr<Controller> controller_;
    std::unique_ptr<Raycast> raycast_;
    std::shared_ptr<Bullet> bullet_;
    std::weak_ptr<MapData> mapData_;

    std::list<CommandData> commandList_;

    std::unique_ptr<state::ModuleNode> moduleNode_;

    friend state::Move;
    friend state::CheckKeyTrg;
    friend state::CheckKey;
    friend state::SetAnim;
    friend state::Stop;
    friend state::CheckRay;
    friend state::CheckG;
    friend state::Fall;
    friend state::Jump;
    friend state::CheckAnim;
    friend state::Attack;
    friend state::Command;
    friend state::CheckAlive;
};

