#include <vector>

using namespace std;

#ifndef FPS
#define FPS 25
#endif

enum class Motion{
    MOTION,
    NO_MOTION
};

template<typename A>
class MockMotionDetector final
{
public:
    void initializeMotionVector ()
    {
        auto index = FPS * 5;
        while (index--)
            mock_motion.emplace_back(Motion::MOTION);

        index = FPS * 5;
        while (index--)
            mock_motion.emplace_back(Motion::NO_MOTION);

        index = FPS * 10;
        while (index--)
            mock_motion.emplace_back(Motion::MOTION);

        index = FPS * 10;
        while (index--)
            mock_motion.emplace_back(Motion::NO_MOTION);
    }

    Motion detect(const A & previous_frame, const A & current_frame){
        ++index;
        return mock_motion[index%mock_motion.size()];
    }
private:
    std::vector<Motion> mock_motion;
//    const std::vector<Motion> mock_motion = {
//    Motion::NO_MOTION,Motion::NO_MOTION,Motion::NO_MOTION,
//    Motion::MOTION,Motion::MOTION, Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,
//    Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,
//    Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::MOTION,Motion::NO_MOTION, Motion::NO_MOTION, Motion::NO_MOTION};
    
    std::uint64_t index = 0;
};
