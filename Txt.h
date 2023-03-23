#pragma once
namespace l1
{
	class Txt
	{
	private:
		char* array = nullptr;
		size_t lines = 0;	
		size_t buf = 0;
	public:
		Txt() = default; 
		Txt(const char* filename); //constructor with 1 argument
		Txt(const Txt& t); //copy constructor
		Txt& operator=(Txt const& other); //copy = operator
		Txt(Txt&& other) noexcept; //move constructor
		Txt& operator=(Txt&& other) noexcept; //move = operator
		~Txt();
		size_t size() const;
		size_t cnt_lines(std::istream& is);
		size_t cnt_lines(const char* filename);
	};
}
