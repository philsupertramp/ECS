//
// Created by phil on 01.10.20.
//

#ifndef ECS_EXAMPLE_H
#define ECS_EXAMPLE_H

#include <ECS/ECS.h>
#include <string>

class Foo
: public ECS::Entity<Foo>
{
    int m_ItCount = 0;
public:
    void Print(){ std::printf(("Foo!" + std::to_string(m_ItCount)).c_str());std::printf("\n");m_ItCount++;}

};

/** COMPONENTS */
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


/* SYSTEMS */
class FooSystem
: public ECS::System<FooSystem>
{
    int m_IterationCount = 0;

    ECS::EntityManager::EntityContainer<Foo>* foos;
public:
    void PreUpdate(float td) override {
        foos = ECS::ECS_Engine->GetEntityManager()->GetEntityContainer<Foo>();
        printf("FooSystem PreUpdate\n");
    }
    void Update(float td) override {
        m_IterationCount = 0;
        for(auto it = foos->begin(); it != foos->end(); ++it){
            it->Print();
            m_IterationCount++;
        }
        printf(("Update called " + std::to_string(m_IterationCount) + " children\n").c_str() );
        m_IterationCount = 0;
    }
    void PostUpdate(float td) override {
        foos = nullptr;
        printf("FooSystem PostUpdate\n");
    }
};

void simpleExample(){

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
}

void systemExample(){
    ECS::ECS_Engine->GetSystemManager()->AddSystem<FooSystem>();

    printf("Expecting no output:\n");
    ECS::ECS_Engine->Update(0.1f);
    // No output
    ECS::ECS_Engine->GetEntityManager()->CreateEntity<Foo>();
    printf("Expecting output:\n");
    ECS::ECS_Engine->Update(0.1f);
    // expects output
}

#endif //ECS_EXAMPLE_H
