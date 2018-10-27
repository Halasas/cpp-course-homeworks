#include "huffman/packer.h"

int main()
{
	while (true) {
		cout << "choose a mod:\n[P]-pack\n[U]-unpack\n[E]-exit\n";
		char mod;
		cin >> mod;
		if (mod == 'e' || mod == 'E')
			return 0;
		if (!((mod == 'e' || mod == 'E') || (mod == 'p' || mod == 'P') || (mod == 'u' || mod == 'U')))
			continue;
		string file_in, file_out;
		cout << "enter file_in:\n";
		cin >> file_in;
		cout << "enter file_out:\n";
		cin >> file_out;
		if (mod == 'p' || mod == 'P') 
		{
			packer::pack(file_in, file_out);
			cout << "Successfully packed!";
		}
		if (mod == 'u' || mod == 'U')
		{
			packer::unpack(file_in, file_out);
			cout << "Successfully unpacked!";
		}
	}
	return 0;
}

