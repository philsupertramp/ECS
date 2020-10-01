#include <ECS/ECS.h>
#include <string>

class Foo
: public ECS::Entity<Foo>
{
public:
    void Print(){ std::printf("Foo!");std::printf("\n"); }

};
class Bar
: public ECS::Component<Bar>
{
public:
    int m_Int = 0;
    void Print() {std::printf(std::to_string(m_Int).c_str());std::printf("\n");}
};
class Baz
: public ECS::Component<Bar>
{
public:
    float m_Float = 0.0f;
    void Print() {std::printf(std::to_string(m_Float).c_str());std::printf("\n");}
};

int main(int nargs, char* argv[]){
    auto foo = (Foo*)ECS::ECS_Engine->GetEntityManager()->CreateEntity<Foo>();
    foo->Print();
    auto originalBar = foo->CreateComponent<Bar>();
    originalBar->m_Int = 10;
    originalBar->Print();
    auto bar = (Bar*)foo->GetComponent<Bar>();
    bar->m_Int = 12;
    bar->Print();

    auto foo2 = (Foo*)ECS::ECS_Engine->GetEntityManager()->CreateEntity<Foo>();
    foo2->Print();
    auto origBar2 = (Bar*)ECS::ECS_Engine->GetComponentManager()->AddComponent<Bar>(foo->GetId());
    origBar2->m_Int = 10;
    origBar2->Print();

    auto firstEntity = ECS::ECS_Engine->GetEntityManager()->GetEntity(foo->GetId());
    auto baz1 = firstEntity->CreateComponent<Baz>();
    baz1->m_Float = 10000.9f;
    baz1->Print();

    ECS::ECS_Engine->GetEntityManager()->DestroyEntity(foo2->GetId());
    ECS::ECS_Engine->Update(0.1f);
    foo2 = (Foo*)ECS::ECS_Engine->GetEntityManager()->GetEntity(foo2->GetId());

    assert(foo2 == nullptr);

    origBar2 = foo2->CreateComponent<Bar>();

    assert(origBar2 == nullptr);

    origBar2 = (Bar*)ECS::ECS_Engine->GetComponentManager()->GetComponent<Bar>(foo2->GetId());

    assert(origBar2 == nullptr);

    return 0;
}