#if !defined (MANAGER)
#define MANAGER

namespace GlassEngine{

	class Manager
	{
	public:
		Manager(){};
		virtual ~Manager() = 0{};

		virtual void Start(){};
		virtual void Update(){};
		virtual void FixedUpdate(){};
		virtual void Stop(){};

	};

}
#endif //MANAGER