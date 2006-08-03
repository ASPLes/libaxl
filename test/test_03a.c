#include <axl.h>
#include <stdio.h>

/* the lookup function  */
int lookups = 0;

bool __find_item (axlPointer _element, axlPointer data)
{
	char * value = _element;
	char * name  = data;

        /* check the name  */
        if (axl_cmp (value, name))
                return true;

        /* it is not the element */
        return false;
}

void test_01_check_key (axlList * list, char * key)
{
	/* find the item */
	char * value = axl_list_lookup (list, __find_item, key);
	
	if (! axl_cmp (value, key)) {
		printf ("ERROR: unable to find %s value, found %s in the list!!!\n", key, value);
		exit (-1);
	}

	lookups++;

	return;
}

/** 
 * @brief Test current libaxl hash implementation.
 *
 * @return true if it works properly or false if not.
 */
bool test_01 () 
{
	axlList * list;
	int       iterator;

	/* create the hash */
	list = axl_list_new (axl_hash_equal_string, NULL);
	
	/* insert data */
	axl_list_add (list, "sword"); 
	axl_list_add (list, "mace");
	axl_list_add (list, "axe");
	axl_list_add (list, "arrow");
	axl_list_add (list, "shield");
	axl_list_add (list, "bag");
	axl_list_add (list, "stone");
	axl_list_add (list, "key");
	axl_list_add (list, "skull");
	axl_list_add (list, "jar");
	axl_list_add (list, "bottle");
	axl_list_add (list, "fairy");
	axl_list_add (list, "potion");
	axl_list_add (list, "water");
	axl_list_add (list, "spoon");
	axl_list_add (list, "book");
	axl_list_add (list, "spear");
	axl_list_add (list, "dagger");
	axl_list_add (list, "katana");
	axl_list_add (list, "helmet");
	axl_list_add (list, "chain");
	axl_list_add (list, "halberd");
	axl_list_add (list, "pipe");
	axl_list_add (list, "hat");
	axl_list_add (list, "eyeofnewt");
	axl_list_add (list, "soup");
	axl_list_add (list, "wolfbane");
	axl_list_add (list, "instantcoffee");
	axl_list_add (list, "bugspray");
	axl_list_add (list, "flint");
	axl_list_add (list, "soap");
	axl_list_add (list, "bones");
	axl_list_add (list, "orb");
	axl_list_add (list, "gold");
	axl_list_add (list, "silver");
	axl_list_add (list, "wine");
	axl_list_add (list, "bread");

	axl_list_add (list, "sword01"); 
	axl_list_add (list, "mace01");
	axl_list_add (list, "axe01");
	axl_list_add (list, "arrow01");
	axl_list_add (list, "shield01");
	axl_list_add (list, "bag01");
	axl_list_add (list, "stone01");
	axl_list_add (list, "key01");
	axl_list_add (list, "skull01");
	axl_list_add (list, "jar01");
	axl_list_add (list, "bottle01");
	axl_list_add (list, "fairy01");
	axl_list_add (list, "potion01");
	axl_list_add (list, "water01");
	axl_list_add (list, "spoon01");
	axl_list_add (list, "book01");
	axl_list_add (list, "spear01");
	axl_list_add (list, "dagger01");
	axl_list_add (list, "katana01");
	axl_list_add (list, "helmet01");
	axl_list_add (list, "chain01");
	axl_list_add (list, "halberd01");
	axl_list_add (list, "pipe01");
	axl_list_add (list, "hat01");
	axl_list_add (list, "eyeofnewt01");
	axl_list_add (list, "soup01");
	axl_list_add (list, "wolfbane01");
	axl_list_add (list, "instantcoffee01");
	axl_list_add (list, "bugspray01");
	axl_list_add (list, "flint01");
	axl_list_add (list, "soap01");
	axl_list_add (list, "bones01");
	axl_list_add (list, "orb01");
	axl_list_add (list, "gold01");
	axl_list_add (list, "silver01");
	axl_list_add (list, "wine01");
	axl_list_add (list, "bread01");

	axl_list_add (list, "sword02"); 
	axl_list_add (list, "mace02");
	axl_list_add (list, "axe02");
	axl_list_add (list, "arrow02");
	axl_list_add (list, "shield02");
	axl_list_add (list, "bag02");
	axl_list_add (list, "stone02");
	axl_list_add (list, "key02");
	axl_list_add (list, "skull02");
	axl_list_add (list, "jar02");
	axl_list_add (list, "bottle02");
	axl_list_add (list, "fairy02");
	axl_list_add (list, "potion02");
	axl_list_add (list, "water02");
	axl_list_add (list, "spoon02");
	axl_list_add (list, "book02");
	axl_list_add (list, "spear02");
	axl_list_add (list, "dagger02");
	axl_list_add (list, "katana02");
	axl_list_add (list, "helmet02");
	axl_list_add (list, "chain02");
	axl_list_add (list, "halberd02");
	axl_list_add (list, "pipe02");
	axl_list_add (list, "hat02");
	axl_list_add (list, "eyeofnewt02");
	axl_list_add (list, "soup02");
	axl_list_add (list, "wolfbane02");
	axl_list_add (list, "instantcoffee02");
	axl_list_add (list, "bugspray02");
	axl_list_add (list, "flint02");
	axl_list_add (list, "soap02");
	axl_list_add (list, "bones02");
	axl_list_add (list, "orb02");
	axl_list_add (list, "gold02");
	axl_list_add (list, "silver02");
	axl_list_add (list, "wine02");
	axl_list_add (list, "bread02");

	axl_list_add (list, "sword03"); 
	axl_list_add (list, "mace03");
	axl_list_add (list, "axe03");
	axl_list_add (list, "arrow03");
	axl_list_add (list, "shield03");
	axl_list_add (list, "bag03");
	axl_list_add (list, "stone03");
	axl_list_add (list, "key03");
	axl_list_add (list, "skull03");
	axl_list_add (list, "jar03");
	axl_list_add (list, "bottle03");
	axl_list_add (list, "fairy03");
	axl_list_add (list, "potion03");
	axl_list_add (list, "water03");
	axl_list_add (list, "spoon03");
	axl_list_add (list, "book03");
	axl_list_add (list, "spear03");
	axl_list_add (list, "dagger03");
	axl_list_add (list, "katana03");
	axl_list_add (list, "helmet03");
	axl_list_add (list, "chain03");
	axl_list_add (list, "halberd03");
	axl_list_add (list, "pipe03");
	axl_list_add (list, "hat03");
	axl_list_add (list, "eyeofnewt03");
	axl_list_add (list, "soup03");
	axl_list_add (list, "wolfbane03");
	axl_list_add (list, "instantcoffee03");
	axl_list_add (list, "bugspray03");
	axl_list_add (list, "flint03");
	axl_list_add (list, "soap03");
	axl_list_add (list, "bones03");
	axl_list_add (list, "orb03");
	axl_list_add (list, "gold03");
	axl_list_add (list, "silver03");
	axl_list_add (list, "wine03");
	axl_list_add (list, "bread03");

	axl_list_add (list, "sword04"); 
	axl_list_add (list, "mace04");
	axl_list_add (list, "axe04");
	axl_list_add (list, "arrow04");
	axl_list_add (list, "shield04");
	axl_list_add (list, "bag04");
	axl_list_add (list, "stone04");
	axl_list_add (list, "key04");
	axl_list_add (list, "skull04");
	axl_list_add (list, "jar04");
	axl_list_add (list, "bottle04");
	axl_list_add (list, "fairy04");
	axl_list_add (list, "potion04");
	axl_list_add (list, "water04");
	axl_list_add (list, "spoon04");
	axl_list_add (list, "book04");
	axl_list_add (list, "spear04");
	axl_list_add (list, "dagger04");
	axl_list_add (list, "katana04");
	axl_list_add (list, "helmet04");
	axl_list_add (list, "chain04");
	axl_list_add (list, "halberd04");
	axl_list_add (list, "pipe04");
	axl_list_add (list, "hat04");
	axl_list_add (list, "eyeofnewt04");
	axl_list_add (list, "soup04");
	axl_list_add (list, "wolfbane04");
	axl_list_add (list, "instantcoffee04");
	axl_list_add (list, "bugspray04");
	axl_list_add (list, "flint04");
	axl_list_add (list, "soap04");
	axl_list_add (list, "bones04");
	axl_list_add (list, "orb04");
	axl_list_add (list, "gold04");
	axl_list_add (list, "silver04");
	axl_list_add (list, "wine04");
	axl_list_add (list, "bread04");

	axl_list_add (list, "sword05"); 
	axl_list_add (list, "mace05");
	axl_list_add (list, "axe05");
	axl_list_add (list, "arrow05");
	axl_list_add (list, "shield05");
	axl_list_add (list, "bag05");
	axl_list_add (list, "stone05");
	axl_list_add (list, "key05");
	axl_list_add (list, "skull05");
	axl_list_add (list, "jar05");
	axl_list_add (list, "bottle05");
	axl_list_add (list, "fairy05");
	axl_list_add (list, "potion05");
	axl_list_add (list, "water05");
	axl_list_add (list, "spoon05");
	axl_list_add (list, "book05");
	axl_list_add (list, "spear05");
	axl_list_add (list, "dagger05");
	axl_list_add (list, "katana05");
	axl_list_add (list, "helmet05");
	axl_list_add (list, "chain05");
	axl_list_add (list, "halberd05");
	axl_list_add (list, "pipe05");
	axl_list_add (list, "hat05");
	axl_list_add (list, "eyeofnewt05");
	axl_list_add (list, "soup05");
	axl_list_add (list, "wolfbane05");
	axl_list_add (list, "instantcoffee05");
	axl_list_add (list, "bugspray05");
	axl_list_add (list, "flint05");
	axl_list_add (list, "soap05");
	axl_list_add (list, "bones05");
	axl_list_add (list, "orb05");
	axl_list_add (list, "gold05");
	axl_list_add (list, "silver05");
	axl_list_add (list, "wine05");
	axl_list_add (list, "bread05");

	axl_list_add (list, "sword06"); 
	axl_list_add (list, "mace06");
	axl_list_add (list, "axe06");
	axl_list_add (list, "arrow06");
	axl_list_add (list, "shield06");
	axl_list_add (list, "bag06");
	axl_list_add (list, "stone06");
	axl_list_add (list, "key06");
	axl_list_add (list, "skull06");
	axl_list_add (list, "jar06");
	axl_list_add (list, "bottle06");
	axl_list_add (list, "fairy06");
	axl_list_add (list, "potion06");
	axl_list_add (list, "water06");
	axl_list_add (list, "spoon06");
	axl_list_add (list, "book06");
	axl_list_add (list, "spear06");
	axl_list_add (list, "dagger06");
	axl_list_add (list, "katana06");
	axl_list_add (list, "helmet06");
	axl_list_add (list, "chain06");
	axl_list_add (list, "halberd06");
	axl_list_add (list, "pipe06");
	axl_list_add (list, "hat06");
	axl_list_add (list, "eyeofnewt06");
	axl_list_add (list, "soup06");
	axl_list_add (list, "wolfbane06");
	axl_list_add (list, "instantcoffee06");
	axl_list_add (list, "bugspray06");
	axl_list_add (list, "flint06");
	axl_list_add (list, "soap06");
	axl_list_add (list, "bones06");
	axl_list_add (list, "orb06");
	axl_list_add (list, "gold06");
	axl_list_add (list, "silver06");
	axl_list_add (list, "wine06");
	axl_list_add (list, "bread06");

	axl_list_add (list, "sword07"); 
	axl_list_add (list, "mace07");
	axl_list_add (list, "axe07");
	axl_list_add (list, "arrow07");
	axl_list_add (list, "shield07");
	axl_list_add (list, "bag07");
	axl_list_add (list, "stone07");
	axl_list_add (list, "key07");
	axl_list_add (list, "skull07");
	axl_list_add (list, "jar07");
	axl_list_add (list, "bottle07");
	axl_list_add (list, "fairy07");
	axl_list_add (list, "potion07");
	axl_list_add (list, "water07");
	axl_list_add (list, "spoon07");
	axl_list_add (list, "book07");
	axl_list_add (list, "spear07");
	axl_list_add (list, "dagger07");
	axl_list_add (list, "katana07");
	axl_list_add (list, "helmet07");
	axl_list_add (list, "chain07");
	axl_list_add (list, "halberd07");
	axl_list_add (list, "pipe07");
	axl_list_add (list, "hat07");
	axl_list_add (list, "eyeofnewt07");
	axl_list_add (list, "soup07");
	axl_list_add (list, "wolfbane07");
	axl_list_add (list, "instantcoffee07");
	axl_list_add (list, "bugspray07");
	axl_list_add (list, "flint07");
	axl_list_add (list, "soap07");
	axl_list_add (list, "bones07");
	axl_list_add (list, "orb07");
	axl_list_add (list, "gold07");
	axl_list_add (list, "silver07");
	axl_list_add (list, "wine07");
	axl_list_add (list, "bread07");

	axl_list_add (list, "sword08"); 
	axl_list_add (list, "mace08");
	axl_list_add (list, "axe08");
	axl_list_add (list, "arrow08");
	axl_list_add (list, "shield08");
	axl_list_add (list, "bag08");
	axl_list_add (list, "stone08");
	axl_list_add (list, "key08");
	axl_list_add (list, "skull08");
	axl_list_add (list, "jar08");
	axl_list_add (list, "bottle08");
	axl_list_add (list, "fairy08");
	axl_list_add (list, "potion08");
	axl_list_add (list, "water08");
	axl_list_add (list, "spoon08");
	axl_list_add (list, "book08");
	axl_list_add (list, "spear08");
	axl_list_add (list, "dagger08");
	axl_list_add (list, "katana08");
	axl_list_add (list, "helmet08");
	axl_list_add (list, "chain08");
	axl_list_add (list, "halberd08");
	axl_list_add (list, "pipe08");
	axl_list_add (list, "hat08");
	axl_list_add (list, "eyeofnewt08");
	axl_list_add (list, "soup08");
	axl_list_add (list, "wolfbane08");
	axl_list_add (list, "instantcoffee08");
	axl_list_add (list, "bugspray08");
	axl_list_add (list, "flint08");
	axl_list_add (list, "soap08");
	axl_list_add (list, "bones08");
	axl_list_add (list, "orb08");
	axl_list_add (list, "gold08");
	axl_list_add (list, "silver08");
	axl_list_add (list, "wine08");
	axl_list_add (list, "bread08");

	axl_list_add (list, "sword09"); 
	axl_list_add (list, "mace09");
	axl_list_add (list, "axe09");
	axl_list_add (list, "arrow09");
	axl_list_add (list, "shield09");
	axl_list_add (list, "bag09");
	axl_list_add (list, "stone09");
	axl_list_add (list, "key09");
	axl_list_add (list, "skull09");
	axl_list_add (list, "jar09");
	axl_list_add (list, "bottle09");
	axl_list_add (list, "fairy09");
	axl_list_add (list, "potion09");
	axl_list_add (list, "water09");
	axl_list_add (list, "spoon09");
	axl_list_add (list, "book09");
	axl_list_add (list, "spear09");
	axl_list_add (list, "dagger09");
	axl_list_add (list, "katana09");
	axl_list_add (list, "helmet09");
	axl_list_add (list, "chain09");
	axl_list_add (list, "halberd09");
	axl_list_add (list, "pipe09");
	axl_list_add (list, "hat09");
	axl_list_add (list, "eyeofnewt09");
	axl_list_add (list, "soup09");
	axl_list_add (list, "wolfbane09");
	axl_list_add (list, "instantcoffee09");
	axl_list_add (list, "bugspray09");
	axl_list_add (list, "flint09");
	axl_list_add (list, "soap09");
	axl_list_add (list, "bones09");
	axl_list_add (list, "orb09");
	axl_list_add (list, "gold09");
	axl_list_add (list, "silver09");
	axl_list_add (list, "wine09");
	axl_list_add (list, "bread09");

	axl_list_add (list, "sword10"); 
	axl_list_add (list, "mace10");
	axl_list_add (list, "axe10");
	axl_list_add (list, "arrow10");
	axl_list_add (list, "shield10");
	axl_list_add (list, "bag10");
	axl_list_add (list, "stone10");
	axl_list_add (list, "key10");
	axl_list_add (list, "skull10");
	axl_list_add (list, "jar10");
	axl_list_add (list, "bottle10");
	axl_list_add (list, "fairy10");
	axl_list_add (list, "potion10");
	axl_list_add (list, "water10");
	axl_list_add (list, "spoon10");
	axl_list_add (list, "book10");
	axl_list_add (list, "spear10");
	axl_list_add (list, "dagger10");
	axl_list_add (list, "katana10");
	axl_list_add (list, "helmet10");
	axl_list_add (list, "chain10");
	axl_list_add (list, "halberd10");
	axl_list_add (list, "pipe10");
	axl_list_add (list, "hat10");
	axl_list_add (list, "eyeofnewt10");
	axl_list_add (list, "soup10");
	axl_list_add (list, "wolfbane10");
	axl_list_add (list, "instantcoffee10");
	axl_list_add (list, "bugspray10");
	axl_list_add (list, "flint10");
	axl_list_add (list, "soap10");
	axl_list_add (list, "bones10");
	axl_list_add (list, "orb10");
	axl_list_add (list, "gold10");
	axl_list_add (list, "silver10");
	axl_list_add (list, "wine10");
	axl_list_add (list, "bread10");

	axl_list_add (list, "sword11"); 
	axl_list_add (list, "mace11");
	axl_list_add (list, "axe11");
	axl_list_add (list, "arrow11");
	axl_list_add (list, "shield11");
	axl_list_add (list, "bag11");
	axl_list_add (list, "stone11");
	axl_list_add (list, "key11");
	axl_list_add (list, "skull11");
	axl_list_add (list, "jar11");
	axl_list_add (list, "bottle11");
	axl_list_add (list, "fairy11");
	axl_list_add (list, "potion11");
	axl_list_add (list, "water11");
	axl_list_add (list, "spoon11");
	axl_list_add (list, "book11");
	axl_list_add (list, "spear11");
	axl_list_add (list, "dagger11");
	axl_list_add (list, "katana11");
	axl_list_add (list, "helmet11");
	axl_list_add (list, "chain11");
	axl_list_add (list, "halberd11");
	axl_list_add (list, "pipe11");
	axl_list_add (list, "hat11");
	axl_list_add (list, "eyeofnewt11");
	axl_list_add (list, "soup11");
	axl_list_add (list, "wolfbane11");
	axl_list_add (list, "instantcoffee11");
	axl_list_add (list, "bugspray11");
	axl_list_add (list, "flint11");
	axl_list_add (list, "soap11");
	axl_list_add (list, "bones11");
	axl_list_add (list, "orb11");
	axl_list_add (list, "gold11");
	axl_list_add (list, "silver11");
	axl_list_add (list, "wine11");
	axl_list_add (list, "bread11");

	/* perform checks */
	iterator = 0;
	while (iterator < 1000) {

		/* get values and check them */
		test_01_check_key (list, "bag");
		test_01_check_key (list, "sword"); 
		test_01_check_key (list, "mace");
		test_01_check_key (list, "axe");
		test_01_check_key (list, "arrow");
		test_01_check_key (list, "shield");
		test_01_check_key (list, "bag");
		test_01_check_key (list, "stone");
		test_01_check_key (list, "key");
		test_01_check_key (list, "skull");
		test_01_check_key (list, "jar");
		test_01_check_key (list, "bottle");
		test_01_check_key (list, "fairy");
		test_01_check_key (list, "potion");
		test_01_check_key (list, "water");
		test_01_check_key (list, "spoon");
		test_01_check_key (list, "book");
		test_01_check_key (list, "spear");
		test_01_check_key (list, "dagger");
		test_01_check_key (list, "katana");
		test_01_check_key (list, "helmet");
		test_01_check_key (list, "chain");
		test_01_check_key (list, "halberd");
		test_01_check_key (list, "pipe");
		test_01_check_key (list, "hat");
		test_01_check_key (list, "eyeofnewt");
		test_01_check_key (list, "soup");
		test_01_check_key (list, "wolfbane");
		test_01_check_key (list, "instantcoffee");
		test_01_check_key (list, "bugspray");
		test_01_check_key (list, "flint");
		test_01_check_key (list, "soap");
		test_01_check_key (list, "bones");
		test_01_check_key (list, "orb");
		test_01_check_key (list, "gold");
		test_01_check_key (list, "silver");
		test_01_check_key (list, "wine");
		test_01_check_key (list, "bread");

		test_01_check_key (list, "bag01");
		test_01_check_key (list, "sword01"); 
		test_01_check_key (list, "mace01");
		test_01_check_key (list, "axe01");
		test_01_check_key (list, "arrow01");
		test_01_check_key (list, "shield01");
		test_01_check_key (list, "bag01");
		test_01_check_key (list, "stone01");
		test_01_check_key (list, "key01");
		test_01_check_key (list, "skull01");
		test_01_check_key (list, "jar01");
		test_01_check_key (list, "bottle01");
		test_01_check_key (list, "fairy01");
		test_01_check_key (list, "potion01");
		test_01_check_key (list, "water01");
		test_01_check_key (list, "spoon01");
		test_01_check_key (list, "book01");
		test_01_check_key (list, "spear01");
		test_01_check_key (list, "dagger01");
		test_01_check_key (list, "katana01");
		test_01_check_key (list, "helmet01");
		test_01_check_key (list, "chain01");
		test_01_check_key (list, "halberd01");
		test_01_check_key (list, "pipe01");
		test_01_check_key (list, "hat01");
		test_01_check_key (list, "eyeofnewt01");
		test_01_check_key (list, "soup01");
		test_01_check_key (list, "wolfbane01");
		test_01_check_key (list, "instantcoffee01");
		test_01_check_key (list, "bugspray01");
		test_01_check_key (list, "flint01");
		test_01_check_key (list, "soap01");
		test_01_check_key (list, "bones01");
		test_01_check_key (list, "orb01");
		test_01_check_key (list, "gold01");
		test_01_check_key (list, "silver01");
		test_01_check_key (list, "wine01");
		test_01_check_key (list, "bread01");

		test_01_check_key (list, "bag02");
		test_01_check_key (list, "sword02"); 
		test_01_check_key (list, "mace02");
		test_01_check_key (list, "axe02");
		test_01_check_key (list, "arrow02");
		test_01_check_key (list, "shield02");
		test_01_check_key (list, "bag02");
		test_01_check_key (list, "stone02");
		test_01_check_key (list, "key02");
		test_01_check_key (list, "skull02");
		test_01_check_key (list, "jar02");
		test_01_check_key (list, "bottle02");
		test_01_check_key (list, "fairy02");
		test_01_check_key (list, "potion02");
		test_01_check_key (list, "water02");
		test_01_check_key (list, "spoon02");
		test_01_check_key (list, "book02");
		test_01_check_key (list, "spear02");
		test_01_check_key (list, "dagger02");
		test_01_check_key (list, "katana02");
		test_01_check_key (list, "helmet02");
		test_01_check_key (list, "chain02");
		test_01_check_key (list, "halberd02");
		test_01_check_key (list, "pipe02");
		test_01_check_key (list, "hat02");
		test_01_check_key (list, "eyeofnewt02");
		test_01_check_key (list, "soup02");
		test_01_check_key (list, "wolfbane02");
		test_01_check_key (list, "instantcoffee02");
		test_01_check_key (list, "bugspray02");
		test_01_check_key (list, "flint02");
		test_01_check_key (list, "soap02");
		test_01_check_key (list, "bones02");
		test_01_check_key (list, "orb02");
		test_01_check_key (list, "gold02");
		test_01_check_key (list, "silver02");
		test_01_check_key (list, "wine02");
		test_01_check_key (list, "bread02");	

		test_01_check_key (list, "bag03");
		test_01_check_key (list, "sword03"); 
		test_01_check_key (list, "mace03");
		test_01_check_key (list, "axe03");
		test_01_check_key (list, "arrow03");
		test_01_check_key (list, "shield03");
		test_01_check_key (list, "bag03");
		test_01_check_key (list, "stone03");
		test_01_check_key (list, "key03");
		test_01_check_key (list, "skull03");
		test_01_check_key (list, "jar03");
		test_01_check_key (list, "bottle03");
		test_01_check_key (list, "fairy03");
		test_01_check_key (list, "potion03");
		test_01_check_key (list, "water03");
		test_01_check_key (list, "spoon03");
		test_01_check_key (list, "book03");
		test_01_check_key (list, "spear03");
		test_01_check_key (list, "dagger03");
		test_01_check_key (list, "katana03");
		test_01_check_key (list, "helmet03");
		test_01_check_key (list, "chain03");
		test_01_check_key (list, "halberd03");
		test_01_check_key (list, "pipe03");
		test_01_check_key (list, "hat03");
		test_01_check_key (list, "eyeofnewt03");
		test_01_check_key (list, "soup03");
		test_01_check_key (list, "wolfbane03");
		test_01_check_key (list, "instantcoffee03");
		test_01_check_key (list, "bugspray03");
		test_01_check_key (list, "flint03");
		test_01_check_key (list, "soap03");
		test_01_check_key (list, "bones03");
		test_01_check_key (list, "orb03");
		test_01_check_key (list, "gold03");
		test_01_check_key (list, "silver03");
		test_01_check_key (list, "wine03");
		test_01_check_key (list, "bread03");	

		test_01_check_key (list, "bag04");
		test_01_check_key (list, "sword04"); 
		test_01_check_key (list, "mace04");
		test_01_check_key (list, "axe04");
		test_01_check_key (list, "arrow04");
		test_01_check_key (list, "shield04");
		test_01_check_key (list, "bag04");
		test_01_check_key (list, "stone04");
		test_01_check_key (list, "key04");
		test_01_check_key (list, "skull04");
		test_01_check_key (list, "jar04");
		test_01_check_key (list, "bottle04");
		test_01_check_key (list, "fairy04");
		test_01_check_key (list, "potion04");
		test_01_check_key (list, "water04");
		test_01_check_key (list, "spoon04");
		test_01_check_key (list, "book04");
		test_01_check_key (list, "spear04");
		test_01_check_key (list, "dagger04");
		test_01_check_key (list, "katana04");
		test_01_check_key (list, "helmet04");
		test_01_check_key (list, "chain04");
		test_01_check_key (list, "halberd04");
		test_01_check_key (list, "pipe04");
		test_01_check_key (list, "hat04");
		test_01_check_key (list, "eyeofnewt04");
		test_01_check_key (list, "soup04");
		test_01_check_key (list, "wolfbane04");
		test_01_check_key (list, "instantcoffee04");
		test_01_check_key (list, "bugspray04");
		test_01_check_key (list, "flint04");
		test_01_check_key (list, "soap04");
		test_01_check_key (list, "bones04");
		test_01_check_key (list, "orb04");
		test_01_check_key (list, "gold04");
		test_01_check_key (list, "silver04");
		test_01_check_key (list, "wine04");
		test_01_check_key (list, "bread04");	

		test_01_check_key (list, "bag05");
		test_01_check_key (list, "sword05"); 
		test_01_check_key (list, "mace05");
		test_01_check_key (list, "axe05");
		test_01_check_key (list, "arrow05");
		test_01_check_key (list, "shield05");
		test_01_check_key (list, "bag05");
		test_01_check_key (list, "stone05");
		test_01_check_key (list, "key05");
		test_01_check_key (list, "skull05");
		test_01_check_key (list, "jar05");
		test_01_check_key (list, "bottle05");
		test_01_check_key (list, "fairy05");
		test_01_check_key (list, "potion05");
		test_01_check_key (list, "water05");
		test_01_check_key (list, "spoon05");
		test_01_check_key (list, "book05");
		test_01_check_key (list, "spear05");
		test_01_check_key (list, "dagger05");
		test_01_check_key (list, "katana05");
		test_01_check_key (list, "helmet05");
		test_01_check_key (list, "chain05");
		test_01_check_key (list, "halberd05");
		test_01_check_key (list, "pipe05");
		test_01_check_key (list, "hat05");
		test_01_check_key (list, "eyeofnewt05");
		test_01_check_key (list, "soup05");
		test_01_check_key (list, "wolfbane05");
		test_01_check_key (list, "instantcoffee05");
		test_01_check_key (list, "bugspray05");
		test_01_check_key (list, "flint05");
		test_01_check_key (list, "soap05");
		test_01_check_key (list, "bones05");
		test_01_check_key (list, "orb05");
		test_01_check_key (list, "gold05");
		test_01_check_key (list, "silver05");
		test_01_check_key (list, "wine05");
		test_01_check_key (list, "bread05");	

		test_01_check_key (list, "bag06");
		test_01_check_key (list, "sword06"); 
		test_01_check_key (list, "mace06");
		test_01_check_key (list, "axe06");
		test_01_check_key (list, "arrow06");
		test_01_check_key (list, "shield06");
		test_01_check_key (list, "bag06");
		test_01_check_key (list, "stone06");
		test_01_check_key (list, "key06");
		test_01_check_key (list, "skull06");
		test_01_check_key (list, "jar06");
		test_01_check_key (list, "bottle06");
		test_01_check_key (list, "fairy06");
		test_01_check_key (list, "potion06");
		test_01_check_key (list, "water06");
		test_01_check_key (list, "spoon06");
		test_01_check_key (list, "book06");
		test_01_check_key (list, "spear06");
		test_01_check_key (list, "dagger06");
		test_01_check_key (list, "katana06");
		test_01_check_key (list, "helmet06");
		test_01_check_key (list, "chain06");
		test_01_check_key (list, "halberd06");
		test_01_check_key (list, "pipe06");
		test_01_check_key (list, "hat06");
		test_01_check_key (list, "eyeofnewt06");
		test_01_check_key (list, "soup06");
		test_01_check_key (list, "wolfbane06");
		test_01_check_key (list, "instantcoffee06");
		test_01_check_key (list, "bugspray06");
		test_01_check_key (list, "flint06");
		test_01_check_key (list, "soap06");
		test_01_check_key (list, "bones06");
		test_01_check_key (list, "orb06");
		test_01_check_key (list, "gold06");
		test_01_check_key (list, "silver06");
		test_01_check_key (list, "wine06");
		test_01_check_key (list, "bread06");	

		test_01_check_key (list, "bag07");
		test_01_check_key (list, "sword07"); 
		test_01_check_key (list, "mace07");
		test_01_check_key (list, "axe07");
		test_01_check_key (list, "arrow07");
		test_01_check_key (list, "shield07");
		test_01_check_key (list, "bag07");
		test_01_check_key (list, "stone07");
		test_01_check_key (list, "key07");
		test_01_check_key (list, "skull07");
		test_01_check_key (list, "jar07");
		test_01_check_key (list, "bottle07");
		test_01_check_key (list, "fairy07");
		test_01_check_key (list, "potion07");
		test_01_check_key (list, "water07");
		test_01_check_key (list, "spoon07");
		test_01_check_key (list, "book07");
		test_01_check_key (list, "spear07");
		test_01_check_key (list, "dagger07");
		test_01_check_key (list, "katana07");
		test_01_check_key (list, "helmet07");
		test_01_check_key (list, "chain07");
		test_01_check_key (list, "halberd07");
		test_01_check_key (list, "pipe07");
		test_01_check_key (list, "hat07");
		test_01_check_key (list, "eyeofnewt07");
		test_01_check_key (list, "soup07");
		test_01_check_key (list, "wolfbane07");
		test_01_check_key (list, "instantcoffee07");
		test_01_check_key (list, "bugspray07");
		test_01_check_key (list, "flint07");
		test_01_check_key (list, "soap07");
		test_01_check_key (list, "bones07");
		test_01_check_key (list, "orb07");
		test_01_check_key (list, "gold07");
		test_01_check_key (list, "silver07");
		test_01_check_key (list, "wine07");
		test_01_check_key (list, "bread07");	

		test_01_check_key (list, "bag08");
		test_01_check_key (list, "sword08"); 
		test_01_check_key (list, "mace08");
		test_01_check_key (list, "axe08");
		test_01_check_key (list, "arrow08");
		test_01_check_key (list, "shield08");
		test_01_check_key (list, "bag08");
		test_01_check_key (list, "stone08");
		test_01_check_key (list, "key08");
		test_01_check_key (list, "skull08");
		test_01_check_key (list, "jar08");
		test_01_check_key (list, "bottle08");
		test_01_check_key (list, "fairy08");
		test_01_check_key (list, "potion08");
		test_01_check_key (list, "water08");
		test_01_check_key (list, "spoon08");
		test_01_check_key (list, "book08");
		test_01_check_key (list, "spear08");
		test_01_check_key (list, "dagger08");
		test_01_check_key (list, "katana08");
		test_01_check_key (list, "helmet08");
		test_01_check_key (list, "chain08");
		test_01_check_key (list, "halberd08");
		test_01_check_key (list, "pipe08");
		test_01_check_key (list, "hat08");
		test_01_check_key (list, "eyeofnewt08");
		test_01_check_key (list, "soup08");
		test_01_check_key (list, "wolfbane08");
		test_01_check_key (list, "instantcoffee08");
		test_01_check_key (list, "bugspray08");
		test_01_check_key (list, "flint08");
		test_01_check_key (list, "soap08");
		test_01_check_key (list, "bones08");
		test_01_check_key (list, "orb08");
		test_01_check_key (list, "gold08");
		test_01_check_key (list, "silver08");
		test_01_check_key (list, "wine08");
		test_01_check_key (list, "bread08");	

		test_01_check_key (list, "bag09");
		test_01_check_key (list, "sword09"); 
		test_01_check_key (list, "mace09");
		test_01_check_key (list, "axe09");
		test_01_check_key (list, "arrow09");
		test_01_check_key (list, "shield09");
		test_01_check_key (list, "bag09");
		test_01_check_key (list, "stone09");
		test_01_check_key (list, "key09");
		test_01_check_key (list, "skull09");
		test_01_check_key (list, "jar09");
		test_01_check_key (list, "bottle09");
		test_01_check_key (list, "fairy09");
		test_01_check_key (list, "potion09");
		test_01_check_key (list, "water09");
		test_01_check_key (list, "spoon09");
		test_01_check_key (list, "book09");
		test_01_check_key (list, "spear09");
		test_01_check_key (list, "dagger09");
		test_01_check_key (list, "katana09");
		test_01_check_key (list, "helmet09");
		test_01_check_key (list, "chain09");
		test_01_check_key (list, "halberd09");
		test_01_check_key (list, "pipe09");
		test_01_check_key (list, "hat09");
		test_01_check_key (list, "eyeofnewt09");
		test_01_check_key (list, "soup09");
		test_01_check_key (list, "wolfbane09");
		test_01_check_key (list, "instantcoffee09");
		test_01_check_key (list, "bugspray09");
		test_01_check_key (list, "flint09");
		test_01_check_key (list, "soap09");
		test_01_check_key (list, "bones09");
		test_01_check_key (list, "orb09");
		test_01_check_key (list, "gold09");
		test_01_check_key (list, "silver09");
		test_01_check_key (list, "wine09");
		test_01_check_key (list, "bread09");	

		test_01_check_key (list, "bag10");
		test_01_check_key (list, "sword10"); 
		test_01_check_key (list, "mace10");
		test_01_check_key (list, "axe10");
		test_01_check_key (list, "arrow10");
		test_01_check_key (list, "shield10");
		test_01_check_key (list, "bag10");
		test_01_check_key (list, "stone10");
		test_01_check_key (list, "key10");
		test_01_check_key (list, "skull10");
		test_01_check_key (list, "jar10");
		test_01_check_key (list, "bottle10");
		test_01_check_key (list, "fairy10");
		test_01_check_key (list, "potion10");
		test_01_check_key (list, "water10");
		test_01_check_key (list, "spoon10");
		test_01_check_key (list, "book10");
		test_01_check_key (list, "spear10");
		test_01_check_key (list, "dagger10");
		test_01_check_key (list, "katana10");
		test_01_check_key (list, "helmet10");
		test_01_check_key (list, "chain10");
		test_01_check_key (list, "halberd10");
		test_01_check_key (list, "pipe10");
		test_01_check_key (list, "hat10");
		test_01_check_key (list, "eyeofnewt10");
		test_01_check_key (list, "soup10");
		test_01_check_key (list, "wolfbane10");
		test_01_check_key (list, "instantcoffee10");
		test_01_check_key (list, "bugspray10");
		test_01_check_key (list, "flint10");
		test_01_check_key (list, "soap10");
		test_01_check_key (list, "bones10");
		test_01_check_key (list, "orb10");
		test_01_check_key (list, "gold10");
		test_01_check_key (list, "silver10");
		test_01_check_key (list, "wine10");
		test_01_check_key (list, "bread10");	

		test_01_check_key (list, "bag11");
		test_01_check_key (list, "sword11"); 
		test_01_check_key (list, "mace11");
		test_01_check_key (list, "axe11");
		test_01_check_key (list, "arrow11");
		test_01_check_key (list, "shield11");
		test_01_check_key (list, "bag11");
		test_01_check_key (list, "stone11");
		test_01_check_key (list, "key11");
		test_01_check_key (list, "skull11");
		test_01_check_key (list, "jar11");
		test_01_check_key (list, "bottle11");
		test_01_check_key (list, "fairy11");
		test_01_check_key (list, "potion11");
		test_01_check_key (list, "water11");
		test_01_check_key (list, "spoon11");
		test_01_check_key (list, "book11");
		test_01_check_key (list, "spear11");
		test_01_check_key (list, "dagger11");
		test_01_check_key (list, "katana11");
		test_01_check_key (list, "helmet11");
		test_01_check_key (list, "chain11");
		test_01_check_key (list, "halberd11");
		test_01_check_key (list, "pipe11");
		test_01_check_key (list, "hat11");
		test_01_check_key (list, "eyeofnewt11");
		test_01_check_key (list, "soup11");
		test_01_check_key (list, "wolfbane11");
		test_01_check_key (list, "instantcoffee11");
		test_01_check_key (list, "bugspray11");
		test_01_check_key (list, "flint11");
		test_01_check_key (list, "soap11");
		test_01_check_key (list, "bones11");
		test_01_check_key (list, "orb11");
		test_01_check_key (list, "gold11");
		test_01_check_key (list, "silver11");
		test_01_check_key (list, "wine11");
		test_01_check_key (list, "bread11");	

	        /* update iterator */
		iterator++;
	}

	printf ("Performed %d lookups on a list with %d items\n", lookups, axl_list_length (list));

	/* destroy the hash */
	axl_list_free (list);

	/* terminated test */
	return true;
}

/** 
 * @brief Perform some operations using some interfaces provided by
 * the libaxl library.
 */
int main (int argc, char ** argv) 
{
	if (test_01 ()) {
		printf ("LibAxl list comparison with hash implementation [   OK   ]\n");
	}else {
		printf ("LibAxl list comparison with hash implementation [ FAILED ]\n");
	}

	return 0;
}
