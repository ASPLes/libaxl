#include <axl.h>
#include <stdio.h>

int lookups = 0;

void test_02_check_key (axlHash * hash, char * key)
{
	char * value = axl_hash_get (hash, key);	
	
	if (! axl_cmp (value, key)) {
		printf ("ERROR: unable to find %s value, found %s in the hash!!!\n", key, value);
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
bool test_02 () 
{
	axlHash * hash;
	int       iterator;

	/* create the hash */
	hash = axl_hash_new (axl_hash_string, axl_hash_equal_string);
	
	/* insert data */
	axl_hash_insert (hash, "sword", "sword"); 
	axl_hash_insert (hash, "mace", "mace");
	axl_hash_insert (hash, "axe", "axe");
	axl_hash_insert (hash, "arrow", "arrow");
	axl_hash_insert (hash, "shield", "shield");
	axl_hash_insert (hash, "bag", "bag");
	axl_hash_insert (hash, "stone", "stone");
	axl_hash_insert (hash, "key", "key");
	axl_hash_insert (hash, "skull", "skull");
	axl_hash_insert (hash, "jar", "jar");
	axl_hash_insert (hash, "bottle", "bottle");
	axl_hash_insert (hash, "fairy", "fairy");
	axl_hash_insert (hash, "potion", "potion");
	axl_hash_insert (hash, "water", "water");
	axl_hash_insert (hash, "spoon", "spoon");
	axl_hash_insert (hash, "book", "book");
	axl_hash_insert (hash, "spear", "spear");
	axl_hash_insert (hash, "dagger", "dagger");
	axl_hash_insert (hash, "katana", "katana");
	axl_hash_insert (hash, "helmet", "helmet");
	axl_hash_insert (hash, "chain", "chain");
	axl_hash_insert (hash, "halberd", "halberd");
	axl_hash_insert (hash, "pipe", "pipe");
	axl_hash_insert (hash, "hat", "hat");
	axl_hash_insert (hash, "eyeofnewt", "eyeofnewt");
	axl_hash_insert (hash, "soup", "soup");
	axl_hash_insert (hash, "wolfbane", "wolfbane");
	axl_hash_insert (hash, "instantcoffee", "instantcoffee");
	axl_hash_insert (hash, "bugspray", "bugspray");
	axl_hash_insert (hash, "flint", "flint");
	axl_hash_insert (hash, "soap", "soap");
	axl_hash_insert (hash, "bones", "bones");
	axl_hash_insert (hash, "orb", "orb");
	axl_hash_insert (hash, "gold", "gold");
	axl_hash_insert (hash, "silver", "silver");
	axl_hash_insert (hash, "wine", "wine");
	axl_hash_insert (hash, "bread", "bread");

	axl_hash_insert (hash, "sword01", "sword01"); 
	axl_hash_insert (hash, "mace01", "mace01");
	axl_hash_insert (hash, "axe01", "axe01");
	axl_hash_insert (hash, "arrow01", "arrow01");
	axl_hash_insert (hash, "shield01", "shield01");
	axl_hash_insert (hash, "bag01", "bag01");
	axl_hash_insert (hash, "stone01", "stone01");
	axl_hash_insert (hash, "key01", "key01");
	axl_hash_insert (hash, "skull01", "skull01");
	axl_hash_insert (hash, "jar01", "jar01");
	axl_hash_insert (hash, "bottle01", "bottle01");
	axl_hash_insert (hash, "fairy01", "fairy01");
	axl_hash_insert (hash, "potion01", "potion01");
	axl_hash_insert (hash, "water01", "water01");
	axl_hash_insert (hash, "spoon01", "spoon01");
	axl_hash_insert (hash, "book01", "book01");
	axl_hash_insert (hash, "spear01", "spear01");
	axl_hash_insert (hash, "dagger01", "dagger01");
	axl_hash_insert (hash, "katana01", "katana01");
	axl_hash_insert (hash, "helmet01", "helmet01");
	axl_hash_insert (hash, "chain01", "chain01");
	axl_hash_insert (hash, "halberd01", "halberd01");
	axl_hash_insert (hash, "pipe01", "pipe01");
	axl_hash_insert (hash, "hat01", "hat01");
	axl_hash_insert (hash, "eyeofnewt01", "eyeofnewt01");
	axl_hash_insert (hash, "soup01", "soup01");
	axl_hash_insert (hash, "wolfbane01", "wolfbane01");
	axl_hash_insert (hash, "instantcoffee01", "instantcoffee01");
	axl_hash_insert (hash, "bugspray01", "bugspray01");
	axl_hash_insert (hash, "flint01", "flint01");
	axl_hash_insert (hash, "soap01", "soap01");
	axl_hash_insert (hash, "bones01", "bones01");
	axl_hash_insert (hash, "orb01", "orb01");
	axl_hash_insert (hash, "gold01", "gold01");
	axl_hash_insert (hash, "silver01", "silver01");
	axl_hash_insert (hash, "wine01", "wine01");
	axl_hash_insert (hash, "bread01", "bread01");

	axl_hash_insert (hash, "sword02", "sword02"); 
	axl_hash_insert (hash, "mace02", "mace02");
	axl_hash_insert (hash, "axe02", "axe02");
	axl_hash_insert (hash, "arrow02", "arrow02");
	axl_hash_insert (hash, "shield02", "shield02");
	axl_hash_insert (hash, "bag02", "bag02");
	axl_hash_insert (hash, "stone02", "stone02");
	axl_hash_insert (hash, "key02", "key02");
	axl_hash_insert (hash, "skull02", "skull02");
	axl_hash_insert (hash, "jar02", "jar02");
	axl_hash_insert (hash, "bottle02", "bottle02");
	axl_hash_insert (hash, "fairy02", "fairy02");
	axl_hash_insert (hash, "potion02", "potion02");
	axl_hash_insert (hash, "water02", "water02");
	axl_hash_insert (hash, "spoon02", "spoon02");
	axl_hash_insert (hash, "book02", "book02");
	axl_hash_insert (hash, "spear02", "spear02");
	axl_hash_insert (hash, "dagger02", "dagger02");
	axl_hash_insert (hash, "katana02", "katana02");
	axl_hash_insert (hash, "helmet02", "helmet02");
	axl_hash_insert (hash, "chain02", "chain02");
	axl_hash_insert (hash, "halberd02", "halberd02");
	axl_hash_insert (hash, "pipe02", "pipe02");
	axl_hash_insert (hash, "hat02", "hat02");
	axl_hash_insert (hash, "eyeofnewt02", "eyeofnewt02");
	axl_hash_insert (hash, "soup02", "soup02");
	axl_hash_insert (hash, "wolfbane02", "wolfbane02");
	axl_hash_insert (hash, "instantcoffee02", "instantcoffee02");
	axl_hash_insert (hash, "bugspray02", "bugspray02");
	axl_hash_insert (hash, "flint02", "flint02");
	axl_hash_insert (hash, "soap02", "soap02");
	axl_hash_insert (hash, "bones02", "bones02");
	axl_hash_insert (hash, "orb02", "orb02");
	axl_hash_insert (hash, "gold02", "gold02");
	axl_hash_insert (hash, "silver02", "silver02");
	axl_hash_insert (hash, "wine02", "wine02");
	axl_hash_insert (hash, "bread02", "bread02");

	axl_hash_insert (hash, "sword03", "sword03"); 
	axl_hash_insert (hash, "mace03", "mace03");
	axl_hash_insert (hash, "axe03", "axe03");
	axl_hash_insert (hash, "arrow03", "arrow03");
	axl_hash_insert (hash, "shield03", "shield03");
	axl_hash_insert (hash, "bag03", "bag03");
	axl_hash_insert (hash, "stone03", "stone03");
	axl_hash_insert (hash, "key03", "key03");
	axl_hash_insert (hash, "skull03", "skull03");
	axl_hash_insert (hash, "jar03", "jar03");
	axl_hash_insert (hash, "bottle03", "bottle03");
	axl_hash_insert (hash, "fairy03", "fairy03");
	axl_hash_insert (hash, "potion03", "potion03");
	axl_hash_insert (hash, "water03", "water03");
	axl_hash_insert (hash, "spoon03", "spoon03");
	axl_hash_insert (hash, "book03", "book03");
	axl_hash_insert (hash, "spear03", "spear03");
	axl_hash_insert (hash, "dagger03", "dagger03");
	axl_hash_insert (hash, "katana03", "katana03");
	axl_hash_insert (hash, "helmet03", "helmet03");
	axl_hash_insert (hash, "chain03", "chain03");
	axl_hash_insert (hash, "halberd03", "halberd03");
	axl_hash_insert (hash, "pipe03", "pipe03");
	axl_hash_insert (hash, "hat03", "hat03");
	axl_hash_insert (hash, "eyeofnewt03", "eyeofnewt03");
	axl_hash_insert (hash, "soup03", "soup03");
	axl_hash_insert (hash, "wolfbane03", "wolfbane03");
	axl_hash_insert (hash, "instantcoffee03", "instantcoffee03");
	axl_hash_insert (hash, "bugspray03", "bugspray03");
	axl_hash_insert (hash, "flint03", "flint03");
	axl_hash_insert (hash, "soap03", "soap03");
	axl_hash_insert (hash, "bones03", "bones03");
	axl_hash_insert (hash, "orb03", "orb03");
	axl_hash_insert (hash, "gold03", "gold03");
	axl_hash_insert (hash, "silver03", "silver03");
	axl_hash_insert (hash, "wine03", "wine03");
	axl_hash_insert (hash, "bread03", "bread03");

	axl_hash_insert (hash, "sword04", "sword04"); 
	axl_hash_insert (hash, "mace04", "mace04");
	axl_hash_insert (hash, "axe04", "axe04");
	axl_hash_insert (hash, "arrow04", "arrow04");
	axl_hash_insert (hash, "shield04", "shield04");
	axl_hash_insert (hash, "bag04", "bag04");
	axl_hash_insert (hash, "stone04", "stone04");
	axl_hash_insert (hash, "key04", "key04");
	axl_hash_insert (hash, "skull04", "skull04");
	axl_hash_insert (hash, "jar04", "jar04");
	axl_hash_insert (hash, "bottle04", "bottle04");
	axl_hash_insert (hash, "fairy04", "fairy04");
	axl_hash_insert (hash, "potion04", "potion04");
	axl_hash_insert (hash, "water04", "water04");
	axl_hash_insert (hash, "spoon04", "spoon04");
	axl_hash_insert (hash, "book04", "book04");
	axl_hash_insert (hash, "spear04", "spear04");
	axl_hash_insert (hash, "dagger04", "dagger04");
	axl_hash_insert (hash, "katana04", "katana04");
	axl_hash_insert (hash, "helmet04", "helmet04");
	axl_hash_insert (hash, "chain04", "chain04");
	axl_hash_insert (hash, "halberd04", "halberd04");
	axl_hash_insert (hash, "pipe04", "pipe04");
	axl_hash_insert (hash, "hat04", "hat04");
	axl_hash_insert (hash, "eyeofnewt04", "eyeofnewt04");
	axl_hash_insert (hash, "soup04", "soup04");
	axl_hash_insert (hash, "wolfbane04", "wolfbane04");
	axl_hash_insert (hash, "instantcoffee04", "instantcoffee04");
	axl_hash_insert (hash, "bugspray04", "bugspray04");
	axl_hash_insert (hash, "flint04", "flint04");
	axl_hash_insert (hash, "soap04", "soap04");
	axl_hash_insert (hash, "bones04", "bones04");
	axl_hash_insert (hash, "orb04", "orb04");
	axl_hash_insert (hash, "gold04", "gold04");
	axl_hash_insert (hash, "silver04", "silver04");
	axl_hash_insert (hash, "wine04", "wine04");
	axl_hash_insert (hash, "bread04", "bread04");

	axl_hash_insert (hash, "sword05", "sword05"); 
	axl_hash_insert (hash, "mace05", "mace05");
	axl_hash_insert (hash, "axe05", "axe05");
	axl_hash_insert (hash, "arrow05", "arrow05");
	axl_hash_insert (hash, "shield05", "shield05");
	axl_hash_insert (hash, "bag05", "bag05");
	axl_hash_insert (hash, "stone05", "stone05");
	axl_hash_insert (hash, "key05", "key05");
	axl_hash_insert (hash, "skull05", "skull05");
	axl_hash_insert (hash, "jar05", "jar05");
	axl_hash_insert (hash, "bottle05", "bottle05");
	axl_hash_insert (hash, "fairy05", "fairy05");
	axl_hash_insert (hash, "potion05", "potion05");
	axl_hash_insert (hash, "water05", "water05");
	axl_hash_insert (hash, "spoon05", "spoon05");
	axl_hash_insert (hash, "book05", "book05");
	axl_hash_insert (hash, "spear05", "spear05");
	axl_hash_insert (hash, "dagger05", "dagger05");
	axl_hash_insert (hash, "katana05", "katana05");
	axl_hash_insert (hash, "helmet05", "helmet05");
	axl_hash_insert (hash, "chain05", "chain05");
	axl_hash_insert (hash, "halberd05", "halberd05");
	axl_hash_insert (hash, "pipe05", "pipe05");
	axl_hash_insert (hash, "hat05", "hat05");
	axl_hash_insert (hash, "eyeofnewt05", "eyeofnewt05");
	axl_hash_insert (hash, "soup05", "soup05");
	axl_hash_insert (hash, "wolfbane05", "wolfbane05");
	axl_hash_insert (hash, "instantcoffee05", "instantcoffee05");
	axl_hash_insert (hash, "bugspray05", "bugspray05");
	axl_hash_insert (hash, "flint05", "flint05");
	axl_hash_insert (hash, "soap05", "soap05");
	axl_hash_insert (hash, "bones05", "bones05");
	axl_hash_insert (hash, "orb05", "orb05");
	axl_hash_insert (hash, "gold05", "gold05");
	axl_hash_insert (hash, "silver05", "silver05");
	axl_hash_insert (hash, "wine05", "wine05");
	axl_hash_insert (hash, "bread05", "bread05");

	axl_hash_insert (hash, "sword06", "sword06"); 
	axl_hash_insert (hash, "mace06", "mace06");
	axl_hash_insert (hash, "axe06", "axe06");
	axl_hash_insert (hash, "arrow06", "arrow06");
	axl_hash_insert (hash, "shield06", "shield06");
	axl_hash_insert (hash, "bag06", "bag06");
	axl_hash_insert (hash, "stone06", "stone06");
	axl_hash_insert (hash, "key06", "key06");
	axl_hash_insert (hash, "skull06", "skull06");
	axl_hash_insert (hash, "jar06", "jar06");
	axl_hash_insert (hash, "bottle06", "bottle06");
	axl_hash_insert (hash, "fairy06", "fairy06");
	axl_hash_insert (hash, "potion06", "potion06");
	axl_hash_insert (hash, "water06", "water06");
	axl_hash_insert (hash, "spoon06", "spoon06");
	axl_hash_insert (hash, "book06", "book06");
	axl_hash_insert (hash, "spear06", "spear06");
	axl_hash_insert (hash, "dagger06", "dagger06");
	axl_hash_insert (hash, "katana06", "katana06");
	axl_hash_insert (hash, "helmet06", "helmet06");
	axl_hash_insert (hash, "chain06", "chain06");
	axl_hash_insert (hash, "halberd06", "halberd06");
	axl_hash_insert (hash, "pipe06", "pipe06");
	axl_hash_insert (hash, "hat06", "hat06");
	axl_hash_insert (hash, "eyeofnewt06", "eyeofnewt06");
	axl_hash_insert (hash, "soup06", "soup06");
	axl_hash_insert (hash, "wolfbane06", "wolfbane06");
	axl_hash_insert (hash, "instantcoffee06", "instantcoffee06");
	axl_hash_insert (hash, "bugspray06", "bugspray06");
	axl_hash_insert (hash, "flint06", "flint06");
	axl_hash_insert (hash, "soap06", "soap06");
	axl_hash_insert (hash, "bones06", "bones06");
	axl_hash_insert (hash, "orb06", "orb06");
	axl_hash_insert (hash, "gold06", "gold06");
	axl_hash_insert (hash, "silver06", "silver06");
	axl_hash_insert (hash, "wine06", "wine06");
	axl_hash_insert (hash, "bread06", "bread06");

	axl_hash_insert (hash, "sword07", "sword07"); 
	axl_hash_insert (hash, "mace07", "mace07");
	axl_hash_insert (hash, "axe07", "axe07");
	axl_hash_insert (hash, "arrow07", "arrow07");
	axl_hash_insert (hash, "shield07", "shield07");
	axl_hash_insert (hash, "bag07", "bag07");
	axl_hash_insert (hash, "stone07", "stone07");
	axl_hash_insert (hash, "key07", "key07");
	axl_hash_insert (hash, "skull07", "skull07");
	axl_hash_insert (hash, "jar07", "jar07");
	axl_hash_insert (hash, "bottle07", "bottle07");
	axl_hash_insert (hash, "fairy07", "fairy07");
	axl_hash_insert (hash, "potion07", "potion07");
	axl_hash_insert (hash, "water07", "water07");
	axl_hash_insert (hash, "spoon07", "spoon07");
	axl_hash_insert (hash, "book07", "book07");
	axl_hash_insert (hash, "spear07", "spear07");
	axl_hash_insert (hash, "dagger07", "dagger07");
	axl_hash_insert (hash, "katana07", "katana07");
	axl_hash_insert (hash, "helmet07", "helmet07");
	axl_hash_insert (hash, "chain07", "chain07");
	axl_hash_insert (hash, "halberd07", "halberd07");
	axl_hash_insert (hash, "pipe07", "pipe07");
	axl_hash_insert (hash, "hat07", "hat07");
	axl_hash_insert (hash, "eyeofnewt07", "eyeofnewt07");
	axl_hash_insert (hash, "soup07", "soup07");
	axl_hash_insert (hash, "wolfbane07", "wolfbane07");
	axl_hash_insert (hash, "instantcoffee07", "instantcoffee07");
	axl_hash_insert (hash, "bugspray07", "bugspray07");
	axl_hash_insert (hash, "flint07", "flint07");
	axl_hash_insert (hash, "soap07", "soap07");
	axl_hash_insert (hash, "bones07", "bones07");
	axl_hash_insert (hash, "orb07", "orb07");
	axl_hash_insert (hash, "gold07", "gold07");
	axl_hash_insert (hash, "silver07", "silver07");
	axl_hash_insert (hash, "wine07", "wine07");
	axl_hash_insert (hash, "bread07", "bread07");

	axl_hash_insert (hash, "sword08", "sword08"); 
	axl_hash_insert (hash, "mace08", "mace08");
	axl_hash_insert (hash, "axe08", "axe08");
	axl_hash_insert (hash, "arrow08", "arrow08");
	axl_hash_insert (hash, "shield08", "shield08");
	axl_hash_insert (hash, "bag08", "bag08");
	axl_hash_insert (hash, "stone08", "stone08");
	axl_hash_insert (hash, "key08", "key08");
	axl_hash_insert (hash, "skull08", "skull08");
	axl_hash_insert (hash, "jar08", "jar08");
	axl_hash_insert (hash, "bottle08", "bottle08");
	axl_hash_insert (hash, "fairy08", "fairy08");
	axl_hash_insert (hash, "potion08", "potion08");
	axl_hash_insert (hash, "water08", "water08");
	axl_hash_insert (hash, "spoon08", "spoon08");
	axl_hash_insert (hash, "book08", "book08");
	axl_hash_insert (hash, "spear08", "spear08");
	axl_hash_insert (hash, "dagger08", "dagger08");
	axl_hash_insert (hash, "katana08", "katana08");
	axl_hash_insert (hash, "helmet08", "helmet08");
	axl_hash_insert (hash, "chain08", "chain08");
	axl_hash_insert (hash, "halberd08", "halberd08");
	axl_hash_insert (hash, "pipe08", "pipe08");
	axl_hash_insert (hash, "hat08", "hat08");
	axl_hash_insert (hash, "eyeofnewt08", "eyeofnewt08");
	axl_hash_insert (hash, "soup08", "soup08");
	axl_hash_insert (hash, "wolfbane08", "wolfbane08");
	axl_hash_insert (hash, "instantcoffee08", "instantcoffee08");
	axl_hash_insert (hash, "bugspray08", "bugspray08");
	axl_hash_insert (hash, "flint08", "flint08");
	axl_hash_insert (hash, "soap08", "soap08");
	axl_hash_insert (hash, "bones08", "bones08");
	axl_hash_insert (hash, "orb08", "orb08");
	axl_hash_insert (hash, "gold08", "gold08");
	axl_hash_insert (hash, "silver08", "silver08");
	axl_hash_insert (hash, "wine08", "wine08");
	axl_hash_insert (hash, "bread08", "bread08");

	axl_hash_insert (hash, "sword09", "sword09"); 
	axl_hash_insert (hash, "mace09", "mace09");
	axl_hash_insert (hash, "axe09", "axe09");
	axl_hash_insert (hash, "arrow09", "arrow09");
	axl_hash_insert (hash, "shield09", "shield09");
	axl_hash_insert (hash, "bag09", "bag09");
	axl_hash_insert (hash, "stone09", "stone09");
	axl_hash_insert (hash, "key09", "key09");
	axl_hash_insert (hash, "skull09", "skull09");
	axl_hash_insert (hash, "jar09", "jar09");
	axl_hash_insert (hash, "bottle09", "bottle09");
	axl_hash_insert (hash, "fairy09", "fairy09");
	axl_hash_insert (hash, "potion09", "potion09");
	axl_hash_insert (hash, "water09", "water09");
	axl_hash_insert (hash, "spoon09", "spoon09");
	axl_hash_insert (hash, "book09", "book09");
	axl_hash_insert (hash, "spear09", "spear09");
	axl_hash_insert (hash, "dagger09", "dagger09");
	axl_hash_insert (hash, "katana09", "katana09");
	axl_hash_insert (hash, "helmet09", "helmet09");
	axl_hash_insert (hash, "chain09", "chain09");
	axl_hash_insert (hash, "halberd09", "halberd09");
	axl_hash_insert (hash, "pipe09", "pipe09");
	axl_hash_insert (hash, "hat09", "hat09");
	axl_hash_insert (hash, "eyeofnewt09", "eyeofnewt09");
	axl_hash_insert (hash, "soup09", "soup09");
	axl_hash_insert (hash, "wolfbane09", "wolfbane09");
	axl_hash_insert (hash, "instantcoffee09", "instantcoffee09");
	axl_hash_insert (hash, "bugspray09", "bugspray09");
	axl_hash_insert (hash, "flint09", "flint09");
	axl_hash_insert (hash, "soap09", "soap09");
	axl_hash_insert (hash, "bones09", "bones09");
	axl_hash_insert (hash, "orb09", "orb09");
	axl_hash_insert (hash, "gold09", "gold09");
	axl_hash_insert (hash, "silver09", "silver09");
	axl_hash_insert (hash, "wine09", "wine09");
	axl_hash_insert (hash, "bread09", "bread09");

	axl_hash_insert (hash, "sword10", "sword10"); 
	axl_hash_insert (hash, "mace10", "mace10");
	axl_hash_insert (hash, "axe10", "axe10");
	axl_hash_insert (hash, "arrow10", "arrow10");
	axl_hash_insert (hash, "shield10", "shield10");
	axl_hash_insert (hash, "bag10", "bag10");
	axl_hash_insert (hash, "stone10", "stone10");
	axl_hash_insert (hash, "key10", "key10");
	axl_hash_insert (hash, "skull10", "skull10");
	axl_hash_insert (hash, "jar10", "jar10");
	axl_hash_insert (hash, "bottle10", "bottle10");
	axl_hash_insert (hash, "fairy10", "fairy10");
	axl_hash_insert (hash, "potion10", "potion10");
	axl_hash_insert (hash, "water10", "water10");
	axl_hash_insert (hash, "spoon10", "spoon10");
	axl_hash_insert (hash, "book10", "book10");
	axl_hash_insert (hash, "spear10", "spear10");
	axl_hash_insert (hash, "dagger10", "dagger10");
	axl_hash_insert (hash, "katana10", "katana10");
	axl_hash_insert (hash, "helmet10", "helmet10");
	axl_hash_insert (hash, "chain10", "chain10");
	axl_hash_insert (hash, "halberd10", "halberd10");
	axl_hash_insert (hash, "pipe10", "pipe10");
	axl_hash_insert (hash, "hat10", "hat10");
	axl_hash_insert (hash, "eyeofnewt10", "eyeofnewt10");
	axl_hash_insert (hash, "soup10", "soup10");
	axl_hash_insert (hash, "wolfbane10", "wolfbane10");
	axl_hash_insert (hash, "instantcoffee10", "instantcoffee10");
	axl_hash_insert (hash, "bugspray10", "bugspray10");
	axl_hash_insert (hash, "flint10", "flint10");
	axl_hash_insert (hash, "soap10", "soap10");
	axl_hash_insert (hash, "bones10", "bones10");
	axl_hash_insert (hash, "orb10", "orb10");
	axl_hash_insert (hash, "gold10", "gold10");
	axl_hash_insert (hash, "silver10", "silver10");
	axl_hash_insert (hash, "wine10", "wine10");
	axl_hash_insert (hash, "bread10", "bread10");

	axl_hash_insert (hash, "sword11", "sword11"); 
	axl_hash_insert (hash, "mace11", "mace11");
	axl_hash_insert (hash, "axe11", "axe11");
	axl_hash_insert (hash, "arrow11", "arrow11");
	axl_hash_insert (hash, "shield11", "shield11");
	axl_hash_insert (hash, "bag11", "bag11");
	axl_hash_insert (hash, "stone11", "stone11");
	axl_hash_insert (hash, "key11", "key11");
	axl_hash_insert (hash, "skull11", "skull11");
	axl_hash_insert (hash, "jar11", "jar11");
	axl_hash_insert (hash, "bottle11", "bottle11");
	axl_hash_insert (hash, "fairy11", "fairy11");
	axl_hash_insert (hash, "potion11", "potion11");
	axl_hash_insert (hash, "water11", "water11");
	axl_hash_insert (hash, "spoon11", "spoon11");
	axl_hash_insert (hash, "book11", "book11");
	axl_hash_insert (hash, "spear11", "spear11");
	axl_hash_insert (hash, "dagger11", "dagger11");
	axl_hash_insert (hash, "katana11", "katana11");
	axl_hash_insert (hash, "helmet11", "helmet11");
	axl_hash_insert (hash, "chain11", "chain11");
	axl_hash_insert (hash, "halberd11", "halberd11");
	axl_hash_insert (hash, "pipe11", "pipe11");
	axl_hash_insert (hash, "hat11", "hat11");
	axl_hash_insert (hash, "eyeofnewt11", "eyeofnewt11");
	axl_hash_insert (hash, "soup11", "soup11");
	axl_hash_insert (hash, "wolfbane11", "wolfbane11");
	axl_hash_insert (hash, "instantcoffee11", "instantcoffee11");
	axl_hash_insert (hash, "bugspray11", "bugspray11");
	axl_hash_insert (hash, "flint11", "flint11");
	axl_hash_insert (hash, "soap11", "soap11");
	axl_hash_insert (hash, "bones11", "bones11");
	axl_hash_insert (hash, "orb11", "orb11");
	axl_hash_insert (hash, "gold11", "gold11");
	axl_hash_insert (hash, "silver11", "silver11");
	axl_hash_insert (hash, "wine11", "wine11");
	axl_hash_insert (hash, "bread11", "bread11");

	/* show hash status */
	axl_hash_show_status  (hash);

	/* perform checks */
	iterator = 0;
	while (iterator < 1000) {

		/* get values and check them */
		test_02_check_key (hash, "bag");
		test_02_check_key (hash, "sword"); 
		test_02_check_key (hash, "mace");
		test_02_check_key (hash, "axe");
		test_02_check_key (hash, "arrow");
		test_02_check_key (hash, "shield");
		test_02_check_key (hash, "bag");
		test_02_check_key (hash, "stone");
		test_02_check_key (hash, "key");
		test_02_check_key (hash, "skull");
		test_02_check_key (hash, "jar");
		test_02_check_key (hash, "bottle");
		test_02_check_key (hash, "fairy");
		test_02_check_key (hash, "potion");
		test_02_check_key (hash, "water");
		test_02_check_key (hash, "spoon");
		test_02_check_key (hash, "book");
		test_02_check_key (hash, "spear");
		test_02_check_key (hash, "dagger");
		test_02_check_key (hash, "katana");
		test_02_check_key (hash, "helmet");
		test_02_check_key (hash, "chain");
		test_02_check_key (hash, "halberd");
		test_02_check_key (hash, "pipe");
		test_02_check_key (hash, "hat");
		test_02_check_key (hash, "eyeofnewt");
		test_02_check_key (hash, "soup");
		test_02_check_key (hash, "wolfbane");
		test_02_check_key (hash, "instantcoffee");
		test_02_check_key (hash, "bugspray");
		test_02_check_key (hash, "flint");
		test_02_check_key (hash, "soap");
		test_02_check_key (hash, "bones");
		test_02_check_key (hash, "orb");
		test_02_check_key (hash, "gold");
		test_02_check_key (hash, "silver");
		test_02_check_key (hash, "wine");
		test_02_check_key (hash, "bread");
		
		test_02_check_key (hash, "bag01");
		test_02_check_key (hash, "sword01"); 
		test_02_check_key (hash, "mace01");
		test_02_check_key (hash, "axe01");
		test_02_check_key (hash, "arrow01");
		test_02_check_key (hash, "shield01");
		test_02_check_key (hash, "bag01");
		test_02_check_key (hash, "stone01");
		test_02_check_key (hash, "key01");
		test_02_check_key (hash, "skull01");
		test_02_check_key (hash, "jar01");
		test_02_check_key (hash, "bottle01");
		test_02_check_key (hash, "fairy01");
		test_02_check_key (hash, "potion01");
		test_02_check_key (hash, "water01");
		test_02_check_key (hash, "spoon01");
		test_02_check_key (hash, "book01");
		test_02_check_key (hash, "spear01");
		test_02_check_key (hash, "dagger01");
		test_02_check_key (hash, "katana01");
		test_02_check_key (hash, "helmet01");
		test_02_check_key (hash, "chain01");
		test_02_check_key (hash, "halberd01");
		test_02_check_key (hash, "pipe01");
		test_02_check_key (hash, "hat01");
		test_02_check_key (hash, "eyeofnewt01");
		test_02_check_key (hash, "soup01");
		test_02_check_key (hash, "wolfbane01");
		test_02_check_key (hash, "instantcoffee01");
		test_02_check_key (hash, "bugspray01");
		test_02_check_key (hash, "flint01");
		test_02_check_key (hash, "soap01");
		test_02_check_key (hash, "bones01");
		test_02_check_key (hash, "orb01");
		test_02_check_key (hash, "gold01");
		test_02_check_key (hash, "silver01");
		test_02_check_key (hash, "wine01");
		test_02_check_key (hash, "bread01");
		
		test_02_check_key (hash, "bag02");
		test_02_check_key (hash, "sword02"); 
		test_02_check_key (hash, "mace02");
		test_02_check_key (hash, "axe02");
		test_02_check_key (hash, "arrow02");
		test_02_check_key (hash, "shield02");
		test_02_check_key (hash, "bag02");
		test_02_check_key (hash, "stone02");
		test_02_check_key (hash, "key02");
		test_02_check_key (hash, "skull02");
		test_02_check_key (hash, "jar02");
		test_02_check_key (hash, "bottle02");
		test_02_check_key (hash, "fairy02");
		test_02_check_key (hash, "potion02");
		test_02_check_key (hash, "water02");
		test_02_check_key (hash, "spoon02");
		test_02_check_key (hash, "book02");
		test_02_check_key (hash, "spear02");
		test_02_check_key (hash, "dagger02");
		test_02_check_key (hash, "katana02");
		test_02_check_key (hash, "helmet02");
		test_02_check_key (hash, "chain02");
		test_02_check_key (hash, "halberd02");
		test_02_check_key (hash, "pipe02");
		test_02_check_key (hash, "hat02");
		test_02_check_key (hash, "eyeofnewt02");
		test_02_check_key (hash, "soup02");
		test_02_check_key (hash, "wolfbane02");
		test_02_check_key (hash, "instantcoffee02");
		test_02_check_key (hash, "bugspray02");
		test_02_check_key (hash, "flint02");
		test_02_check_key (hash, "soap02");
		test_02_check_key (hash, "bones02");
		test_02_check_key (hash, "orb02");
		test_02_check_key (hash, "gold02");
		test_02_check_key (hash, "silver02");
		test_02_check_key (hash, "wine02");
		test_02_check_key (hash, "bread02");

		test_02_check_key (hash, "bag03");
		test_02_check_key (hash, "sword03"); 
		test_02_check_key (hash, "mace03");
		test_02_check_key (hash, "axe03");
		test_02_check_key (hash, "arrow03");
		test_02_check_key (hash, "shield03");
		test_02_check_key (hash, "bag03");
		test_02_check_key (hash, "stone03");
		test_02_check_key (hash, "key03");
		test_02_check_key (hash, "skull03");
		test_02_check_key (hash, "jar03");
		test_02_check_key (hash, "bottle03");
		test_02_check_key (hash, "fairy03");
		test_02_check_key (hash, "potion03");
		test_02_check_key (hash, "water03");
		test_02_check_key (hash, "spoon03");
		test_02_check_key (hash, "book03");
		test_02_check_key (hash, "spear03");
		test_02_check_key (hash, "dagger03");
		test_02_check_key (hash, "katana03");
		test_02_check_key (hash, "helmet03");
		test_02_check_key (hash, "chain03");
		test_02_check_key (hash, "halberd03");
		test_02_check_key (hash, "pipe03");
		test_02_check_key (hash, "hat03");
		test_02_check_key (hash, "eyeofnewt03");
		test_02_check_key (hash, "soup03");
		test_02_check_key (hash, "wolfbane03");
		test_02_check_key (hash, "instantcoffee03");
		test_02_check_key (hash, "bugspray03");
		test_02_check_key (hash, "flint03");
		test_02_check_key (hash, "soap03");
		test_02_check_key (hash, "bones03");
		test_02_check_key (hash, "orb03");
		test_02_check_key (hash, "gold03");
		test_02_check_key (hash, "silver03");
		test_02_check_key (hash, "wine03");
		test_02_check_key (hash, "bread03");

		test_02_check_key (hash, "bag04");
		test_02_check_key (hash, "sword04"); 
		test_02_check_key (hash, "mace04");
		test_02_check_key (hash, "axe04");
		test_02_check_key (hash, "arrow04");
		test_02_check_key (hash, "shield04");
		test_02_check_key (hash, "bag04");
		test_02_check_key (hash, "stone04");
		test_02_check_key (hash, "key04");
		test_02_check_key (hash, "skull04");
		test_02_check_key (hash, "jar04");
		test_02_check_key (hash, "bottle04");
		test_02_check_key (hash, "fairy04");
		test_02_check_key (hash, "potion04");
		test_02_check_key (hash, "water04");
		test_02_check_key (hash, "spoon04");
		test_02_check_key (hash, "book04");
		test_02_check_key (hash, "spear04");
		test_02_check_key (hash, "dagger04");
		test_02_check_key (hash, "katana04");
		test_02_check_key (hash, "helmet04");
		test_02_check_key (hash, "chain04");
		test_02_check_key (hash, "halberd04");
		test_02_check_key (hash, "pipe04");
		test_02_check_key (hash, "hat04");
		test_02_check_key (hash, "eyeofnewt04");
		test_02_check_key (hash, "soup04");
		test_02_check_key (hash, "wolfbane04");
		test_02_check_key (hash, "instantcoffee04");
		test_02_check_key (hash, "bugspray04");
		test_02_check_key (hash, "flint04");
		test_02_check_key (hash, "soap04");
		test_02_check_key (hash, "bones04");
		test_02_check_key (hash, "orb04");
		test_02_check_key (hash, "gold04");
		test_02_check_key (hash, "silver04");
		test_02_check_key (hash, "wine04");
		test_02_check_key (hash, "bread04");

		test_02_check_key (hash, "bag05");
		test_02_check_key (hash, "sword05"); 
		test_02_check_key (hash, "mace05");
		test_02_check_key (hash, "axe05");
		test_02_check_key (hash, "arrow05");
		test_02_check_key (hash, "shield05");
		test_02_check_key (hash, "bag05");
		test_02_check_key (hash, "stone05");
		test_02_check_key (hash, "key05");
		test_02_check_key (hash, "skull05");
		test_02_check_key (hash, "jar05");
		test_02_check_key (hash, "bottle05");
		test_02_check_key (hash, "fairy05");
		test_02_check_key (hash, "potion05");
		test_02_check_key (hash, "water05");
		test_02_check_key (hash, "spoon05");
		test_02_check_key (hash, "book05");
		test_02_check_key (hash, "spear05");
		test_02_check_key (hash, "dagger05");
		test_02_check_key (hash, "katana05");
		test_02_check_key (hash, "helmet05");
		test_02_check_key (hash, "chain05");
		test_02_check_key (hash, "halberd05");
		test_02_check_key (hash, "pipe05");
		test_02_check_key (hash, "hat05");
		test_02_check_key (hash, "eyeofnewt05");
		test_02_check_key (hash, "soup05");
		test_02_check_key (hash, "wolfbane05");
		test_02_check_key (hash, "instantcoffee05");
		test_02_check_key (hash, "bugspray05");
		test_02_check_key (hash, "flint05");
		test_02_check_key (hash, "soap05");
		test_02_check_key (hash, "bones05");
		test_02_check_key (hash, "orb05");
		test_02_check_key (hash, "gold05");
		test_02_check_key (hash, "silver05");
		test_02_check_key (hash, "wine05");
		test_02_check_key (hash, "bread05");

		test_02_check_key (hash, "bag06");
		test_02_check_key (hash, "sword06"); 
		test_02_check_key (hash, "mace06");
		test_02_check_key (hash, "axe06");
		test_02_check_key (hash, "arrow06");
		test_02_check_key (hash, "shield06");
		test_02_check_key (hash, "bag06");
		test_02_check_key (hash, "stone06");
		test_02_check_key (hash, "key06");
		test_02_check_key (hash, "skull06");
		test_02_check_key (hash, "jar06");
		test_02_check_key (hash, "bottle06");
		test_02_check_key (hash, "fairy06");
		test_02_check_key (hash, "potion06");
		test_02_check_key (hash, "water06");
		test_02_check_key (hash, "spoon06");
		test_02_check_key (hash, "book06");
		test_02_check_key (hash, "spear06");
		test_02_check_key (hash, "dagger06");
		test_02_check_key (hash, "katana06");
		test_02_check_key (hash, "helmet06");
		test_02_check_key (hash, "chain06");
		test_02_check_key (hash, "halberd06");
		test_02_check_key (hash, "pipe06");
		test_02_check_key (hash, "hat06");
		test_02_check_key (hash, "eyeofnewt06");
		test_02_check_key (hash, "soup06");
		test_02_check_key (hash, "wolfbane06");
		test_02_check_key (hash, "instantcoffee06");
		test_02_check_key (hash, "bugspray06");
		test_02_check_key (hash, "flint06");
		test_02_check_key (hash, "soap06");
		test_02_check_key (hash, "bones06");
		test_02_check_key (hash, "orb06");
		test_02_check_key (hash, "gold06");
		test_02_check_key (hash, "silver06");
		test_02_check_key (hash, "wine06");
		test_02_check_key (hash, "bread06");

		test_02_check_key (hash, "bag07");
		test_02_check_key (hash, "sword07"); 
		test_02_check_key (hash, "mace07");
		test_02_check_key (hash, "axe07");
		test_02_check_key (hash, "arrow07");
		test_02_check_key (hash, "shield07");
		test_02_check_key (hash, "bag07");
		test_02_check_key (hash, "stone07");
		test_02_check_key (hash, "key07");
		test_02_check_key (hash, "skull07");
		test_02_check_key (hash, "jar07");
		test_02_check_key (hash, "bottle07");
		test_02_check_key (hash, "fairy07");
		test_02_check_key (hash, "potion07");
		test_02_check_key (hash, "water07");
		test_02_check_key (hash, "spoon07");
		test_02_check_key (hash, "book07");
		test_02_check_key (hash, "spear07");
		test_02_check_key (hash, "dagger07");
		test_02_check_key (hash, "katana07");
		test_02_check_key (hash, "helmet07");
		test_02_check_key (hash, "chain07");
		test_02_check_key (hash, "halberd07");
		test_02_check_key (hash, "pipe07");
		test_02_check_key (hash, "hat07");
		test_02_check_key (hash, "eyeofnewt07");
		test_02_check_key (hash, "soup07");
		test_02_check_key (hash, "wolfbane07");
		test_02_check_key (hash, "instantcoffee07");
		test_02_check_key (hash, "bugspray07");
		test_02_check_key (hash, "flint07");
		test_02_check_key (hash, "soap07");
		test_02_check_key (hash, "bones07");
		test_02_check_key (hash, "orb07");
		test_02_check_key (hash, "gold07");
		test_02_check_key (hash, "silver07");
		test_02_check_key (hash, "wine07");
		test_02_check_key (hash, "bread07");

		test_02_check_key (hash, "bag08");
		test_02_check_key (hash, "sword08"); 
		test_02_check_key (hash, "mace08");
		test_02_check_key (hash, "axe08");
		test_02_check_key (hash, "arrow08");
		test_02_check_key (hash, "shield08");
		test_02_check_key (hash, "bag08");
		test_02_check_key (hash, "stone08");
		test_02_check_key (hash, "key08");
		test_02_check_key (hash, "skull08");
		test_02_check_key (hash, "jar08");
		test_02_check_key (hash, "bottle08");
		test_02_check_key (hash, "fairy08");
		test_02_check_key (hash, "potion08");
		test_02_check_key (hash, "water08");
		test_02_check_key (hash, "spoon08");
		test_02_check_key (hash, "book08");
		test_02_check_key (hash, "spear08");
		test_02_check_key (hash, "dagger08");
		test_02_check_key (hash, "katana08");
		test_02_check_key (hash, "helmet08");
		test_02_check_key (hash, "chain08");
		test_02_check_key (hash, "halberd08");
		test_02_check_key (hash, "pipe08");
		test_02_check_key (hash, "hat08");
		test_02_check_key (hash, "eyeofnewt08");
		test_02_check_key (hash, "soup08");
		test_02_check_key (hash, "wolfbane08");
		test_02_check_key (hash, "instantcoffee08");
		test_02_check_key (hash, "bugspray08");
		test_02_check_key (hash, "flint08");
		test_02_check_key (hash, "soap08");
		test_02_check_key (hash, "bones08");
		test_02_check_key (hash, "orb08");
		test_02_check_key (hash, "gold08");
		test_02_check_key (hash, "silver08");
		test_02_check_key (hash, "wine08");
		test_02_check_key (hash, "bread08");

		test_02_check_key (hash, "bag09");
		test_02_check_key (hash, "sword09"); 
		test_02_check_key (hash, "mace09");
		test_02_check_key (hash, "axe09");
		test_02_check_key (hash, "arrow09");
		test_02_check_key (hash, "shield09");
		test_02_check_key (hash, "bag09");
		test_02_check_key (hash, "stone09");
		test_02_check_key (hash, "key09");
		test_02_check_key (hash, "skull09");
		test_02_check_key (hash, "jar09");
		test_02_check_key (hash, "bottle09");
		test_02_check_key (hash, "fairy09");
		test_02_check_key (hash, "potion09");
		test_02_check_key (hash, "water09");
		test_02_check_key (hash, "spoon09");
		test_02_check_key (hash, "book09");
		test_02_check_key (hash, "spear09");
		test_02_check_key (hash, "dagger09");
		test_02_check_key (hash, "katana09");
		test_02_check_key (hash, "helmet09");
		test_02_check_key (hash, "chain09");
		test_02_check_key (hash, "halberd09");
		test_02_check_key (hash, "pipe09");
		test_02_check_key (hash, "hat09");
		test_02_check_key (hash, "eyeofnewt09");
		test_02_check_key (hash, "soup09");
		test_02_check_key (hash, "wolfbane09");
		test_02_check_key (hash, "instantcoffee09");
		test_02_check_key (hash, "bugspray09");
		test_02_check_key (hash, "flint09");
		test_02_check_key (hash, "soap09");
		test_02_check_key (hash, "bones09");
		test_02_check_key (hash, "orb09");
		test_02_check_key (hash, "gold09");
		test_02_check_key (hash, "silver09");
		test_02_check_key (hash, "wine09");
		test_02_check_key (hash, "bread09");

		test_02_check_key (hash, "bag10");
		test_02_check_key (hash, "sword10"); 
		test_02_check_key (hash, "mace10");
		test_02_check_key (hash, "axe10");
		test_02_check_key (hash, "arrow10");
		test_02_check_key (hash, "shield10");
		test_02_check_key (hash, "bag10");
		test_02_check_key (hash, "stone10");
		test_02_check_key (hash, "key10");
		test_02_check_key (hash, "skull10");
		test_02_check_key (hash, "jar10");
		test_02_check_key (hash, "bottle10");
		test_02_check_key (hash, "fairy10");
		test_02_check_key (hash, "potion10");
		test_02_check_key (hash, "water10");
		test_02_check_key (hash, "spoon10");
		test_02_check_key (hash, "book10");
		test_02_check_key (hash, "spear10");
		test_02_check_key (hash, "dagger10");
		test_02_check_key (hash, "katana10");
		test_02_check_key (hash, "helmet10");
		test_02_check_key (hash, "chain10");
		test_02_check_key (hash, "halberd10");
		test_02_check_key (hash, "pipe10");
		test_02_check_key (hash, "hat10");
		test_02_check_key (hash, "eyeofnewt10");
		test_02_check_key (hash, "soup10");
		test_02_check_key (hash, "wolfbane10");
		test_02_check_key (hash, "instantcoffee10");
		test_02_check_key (hash, "bugspray10");
		test_02_check_key (hash, "flint10");
		test_02_check_key (hash, "soap10");
		test_02_check_key (hash, "bones10");
		test_02_check_key (hash, "orb10");
		test_02_check_key (hash, "gold10");
		test_02_check_key (hash, "silver10");
		test_02_check_key (hash, "wine10");
		test_02_check_key (hash, "bread10");

		test_02_check_key (hash, "bag11");
		test_02_check_key (hash, "sword11"); 
		test_02_check_key (hash, "mace11");
		test_02_check_key (hash, "axe11");
		test_02_check_key (hash, "arrow11");
		test_02_check_key (hash, "shield11");
		test_02_check_key (hash, "bag11");
		test_02_check_key (hash, "stone11");
		test_02_check_key (hash, "key11");
		test_02_check_key (hash, "skull11");
		test_02_check_key (hash, "jar11");
		test_02_check_key (hash, "bottle11");
		test_02_check_key (hash, "fairy11");
		test_02_check_key (hash, "potion11");
		test_02_check_key (hash, "water11");
		test_02_check_key (hash, "spoon11");
		test_02_check_key (hash, "book11");
		test_02_check_key (hash, "spear11");
		test_02_check_key (hash, "dagger11");
		test_02_check_key (hash, "katana11");
		test_02_check_key (hash, "helmet11");
		test_02_check_key (hash, "chain11");
		test_02_check_key (hash, "halberd11");
		test_02_check_key (hash, "pipe11");
		test_02_check_key (hash, "hat11");
		test_02_check_key (hash, "eyeofnewt11");
		test_02_check_key (hash, "soup11");
		test_02_check_key (hash, "wolfbane11");
		test_02_check_key (hash, "instantcoffee11");
		test_02_check_key (hash, "bugspray11");
		test_02_check_key (hash, "flint11");
		test_02_check_key (hash, "soap11");
		test_02_check_key (hash, "bones11");
		test_02_check_key (hash, "orb11");
		test_02_check_key (hash, "gold11");
		test_02_check_key (hash, "silver11");
		test_02_check_key (hash, "wine11");
		test_02_check_key (hash, "bread11");

		/* update iterator */
		iterator++;
	}

	printf ("performed %d lookups in a hash with %d items\n", 
		lookups, axl_hash_items (hash));

	/* destroy the hash */
	axl_hash_free (hash);

	/* terminated test */
	return true;
}

axlPointer test_01_copy_key (axlPointer key, axlDestroyFunc key_destroy, axlPointer data, axlDestroyFunc data_destroy)
{
	if (key_destroy == NULL)
		return key;

	return axl_strdup (key);
}

axlPointer test_01_copy_value (axlPointer key, axlDestroyFunc key_destroy, axlPointer data, axlDestroyFunc data_destroy)
{
	if (data_destroy == NULL)
		return data;

	return axl_strdup (data);
}


/** 
 * @brief Checks normal hash operations.
 * 
 * 
 * @return \ref true if it was ok.
 */
bool test_01 () 
{
	axlHash    * hash;
	axlHash    * hash2;
	axlPointer * data;

	/* create a hash */
	hash = axl_hash_new (axl_hash_string, axl_hash_equal_string);

	/* perform operations on empty state */
	if (axl_hash_exists (hash, "value")) {
		printf ("Found that the hash returns that item exist but it is empty\n");
		return false;
	}

	/* perform a remove operation */
	axl_hash_remove (hash, "value");

	/* get data associated */
	data = axl_hash_get (hash, "value");
	if (data != NULL) {
		printf ("Found that the hash returns that item exist but it is empty\n");
		return false;
	}

	/* add items to the hash */
	axl_hash_insert_full (hash, "value", NULL, axl_strdup ("value"), axl_free);
	axl_hash_insert_full (hash, "value2", NULL, axl_strdup ("value2"), axl_free);
	axl_hash_insert_full (hash, "value3", NULL, axl_strdup ("value3"), axl_free);

	/* check items stored */
	test_02_check_key (hash, "value");
	test_02_check_key (hash, "value2");
	test_02_check_key (hash, "value3");

	/* copy the hash */
	hash2 = axl_hash_copy (hash, test_01_copy_key, test_01_copy_value);

	/* check items stored */
	test_02_check_key (hash2, "value");
	test_02_check_key (hash2, "value2");
	test_02_check_key (hash2, "value3");
	

	/* destroy the hash */
	axl_hash_free (hash);

	/* destroy the hash2 */
	axl_hash_free (hash2);

	return true;
}

/** 
 * @brief Perform some operations using some interfaces provided by
 * the libaxl library.
 */
int main (int argc, char ** argv) 
{
	if (test_01 ()) {
		printf ("LibAxl hash implementation [   OK   ]\n");
	}else {
		printf ("LibAxl hash implementation [ FAILED ]\n");
	}

	if (test_02 ()) {
		printf ("LibAxl hash implementation (lookup) [   OK   ]\n");
	}else {
		printf ("LibAxl hash implementation (lookup) [ FAILED ]\n");
	}

	return 0;
}
