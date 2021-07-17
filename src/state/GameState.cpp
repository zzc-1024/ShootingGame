#include "GameState.h"

GameState::GameState(RenderWindow* window, Font font, stack<State*>* states) :
	State(window, font, states)
{
    const int level[] =
    {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    };

    width = 16 * 32;
    height = 8 * 32;

    Vector2u tileSize(32u, 32u);
    tMap.setTile(tileSize, level, 16u, 8u);

    player.shape.setPosition(width / 2, height / 2);

    texture.loadFromFile("res/Tileset.png");
    fireBullet.loadFromFile("res/fireball.png");

    player.loadTexture(texture, 192, 106, 16, 22, 4);
    ai.loadTexture(texture, 432, 80, 16, 16, 4);

    ai.shape.setPosition(-width / 2, rand() % 600);
    aiList.push_back(ai);

    ai.shape.setPosition(width * 1.5, rand() % 600);
    aiList.push_back(ai);

    ai.shape.setPosition(rand() % 800, -height / 2);
    aiList.push_back(ai);

    ai.shape.setPosition(rand() % 800, height * 1.5);
    aiList.push_back(ai);

    flushAI = 0.f;

    view = window->getView();
    view.setCenter(width / 2, height / 2);

    shootBuffer.loadFromFile("res/shoot.wav");
    shoot.setBuffer(shootBuffer);

    score = 0;
}

GameState::~GameState()
{//尝试解决内存泄漏
    list<AI>().swap(aiList);
    list<Bullet>().swap(bulletList);
}

void GameState::update(float dt)
{
    this->dt = dt;

    //player响应移动指令
    if (Keyboard::isKeyPressed(Keyboard::A))
        player.shape.move(Vector2f(-player.speed * dt, 0));
    if (Keyboard::isKeyPressed(Keyboard::D))
        player.shape.move(Vector2f(player.speed * dt, 0));
    if (Keyboard::isKeyPressed(Keyboard::W))
        player.shape.move(Vector2f(0, -player.speed * dt));
    if (Keyboard::isKeyPressed(Keyboard::S))
        player.shape.move(Vector2f(0, player.speed * dt));
    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
        this->isEnd = true;
        //window->close();
        //return score;
    }

    cout << player.shape.getPosition().x << ' ';

    Vector2f pPos = player.shape.getPosition();

    if (pPos.x < 0)
        pPos.x = 0;
    else if (pPos.x > width)
        pPos.x = width;
    if (pPos.y < 0)
        pPos.y = 0;
    else if (pPos.y > height)
        pPos.y = height;

    player.shape.setPosition(pPos);

    cout << player.shape.getPosition().x << endl;

    view.setCenter(pPos);
    this->window->setView(view);

    //AI进行移动
    for (auto& it : aiList)
    {
        Vector2f pos;
        pos = it.shape.getPosition();
        float dx = pPos.x - pos.x;
        if (dx >= 0)
            it.isReverse = false;
        else
            it.isReverse = true;
        float dy = pPos.y - pos.y;
        float angle;
        if (dx == 0)
            angle = pi / 2;
        else
            angle = abs(atan(dy / dx));

        it.shape.move(
            dx / abs(dx) * it.speed * cos(angle) * dt,
            dy / abs(dy) * it.speed * sin(angle) * dt
        );
    }

    player.cdRemain -= dt;

    updateMouse();

    float dx = mPos.x - pPos.x;
    float dy = mPos.y - pPos.y;

    if (dx >= 0)
        player.isReverse = false;
    else
        player.isReverse = true;
    if (player.cdRemain < 0.f)
        player.cdRemain = 0.f;
    if (player.cdRemain == 0.f && Mouse::isButtonPressed(Mouse::Left))
    {
        shoot.play();
        player.cdRemain = player.cd;
        float angle;
        if (dx == 0)
            if (dy > 0)
                angle = pi / 2;
            else
                angle = -pi / 2;
        else
        {
            angle = atan(dy / dx);
            if (dx < 0)
                angle = angle + pi;
        }
        Bullet tmp;
        tmp.angle = angle;
        tmp.loadTexture(fireBullet, 0, 0, 26, 10, 10);
        tmp.shape.setPosition(pPos);
        bulletList.push_back(tmp);
    }

    //删除子弹
    while (!bulletList.empty() && bulletList.front().life <= 0)
        bulletList.pop_front();

    //更新子弹
    for (auto it = bulletList.begin(); it != bulletList.end();)
    {
        it->shape.move(it->speed * cos(it->angle) * dt, it->speed * sin(it->angle) * dt);
        it->life -= dt;
        bool flag = true;
        //检测子弹是否碰到敌人
        for (auto tmp = aiList.begin(); tmp != aiList.end();)
        {
            if (it->shape.getGlobalBounds().intersects(tmp->shape.getGlobalBounds()))
            {
                bulletList.erase(it++);
                flag = false;
                tmp->hp -= player.attack;
                if (tmp->hp <= 0)
                {
                    aiList.erase(tmp++);
                    score++;
                }
                break;
            }
            else
            {
                tmp++;
            }
        }
        if (flag)
            it++;
    }

    //检测敌人是否碰到玩家
    for (auto it = aiList.begin(); it != aiList.end();)
    {
        if (it->shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
        {
            player.hp -= it->attack;
            aiList.erase(it++);
            if (player.hp <= 0)
                this->isEnd = true;
        }
        else
            it++;
    }

    flushAI += dt;
    while (flushAI >= 0.1f)
    {
        flushAI -= 0.1f;
        ai.shape.setPosition(-width / 2, rand() % 600);
        if (rand() % 1000 < 10 + score)
            aiList.push_back(ai);
        ai.shape.setPosition(width * 1.5, rand() % 600);
        if (rand() % 1000 < 10 + score)
            aiList.push_back(ai);
        ai.shape.setPosition(rand() % 800, -height / 2);
        if (rand() % 1000 < 10 + score)
            aiList.push_back(ai);
        ai.shape.setPosition(rand() % 800, height * 1.5);
        if (rand() % 1000 < 10 + score)
            aiList.push_back(ai);
    }
}

void GameState::draw(RenderStates states)
{
    window->draw(tMap);
    player.update(dt);
    window->draw(player);
    for (auto& it : aiList)
    	window->draw(it.shape);
    for (auto& it : aiList)
    {
        it.update(dt);
        window->draw(it);
    }
    for (auto& it : bulletList)
        window->draw(it.shape);
    for (auto& it : bulletList)
        window->draw(it.updateState(dt));
}
