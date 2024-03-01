class System{
public:
    const short priority;
    virtual void update(float dt);
protected:
    bool enabled;
};

