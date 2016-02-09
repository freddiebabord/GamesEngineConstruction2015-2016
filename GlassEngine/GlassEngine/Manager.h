#if !defined (MANAGER)
#define MANAGER

namespace GlassEngine{

	class Manager
	{
	public:
		Manager(){};
		virtual ~Manager() = 0{};

		virtual void Start(){};
		virtual void Update(){ HAPI->UserMessage("Im right?", "GlassEngine");  };
		virtual void FixedUpdate(){};
		virtual void Stop(){};

	};

}
#endif //MANAGER