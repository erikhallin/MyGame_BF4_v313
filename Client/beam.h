#ifndef BEAM_H
#define BEAM_H

class beam
{
    protected:
              
    public:
        bool  m_active;
        float m_age;
        float m_start_pos[3];
        float m_end_pos[3];
        
        beam();
        void newBeam(float data[]);
        void update(float);
        void drawBeam(void);
      
};

#endif
