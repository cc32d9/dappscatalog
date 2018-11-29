<h1 class="contract">addvoucher</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)
* `contract` (token contract name)
* `price` (promotion price for the first entry in the catalog)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to set a special price for a new entry by a specific owner. It shall
have no other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">blacklist</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)

### Intent

INTENT. The intention of the author and the invoker of this contract is
to delete all records of specified owner, refund 90% of payments, and
mark the owner as blacklisted, so that no new entries could be added by
that user. It shall have no other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">claimrefund</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to remove all data records belonging to the owner and refund all paid
amounts, deducting 10% commission. It shall have no other effect.

### Term
TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">delattr</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)
* `tokenname` (token name or emty string)
* `key` (key name, up to 12 symbols)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to delete an attribute from a specified data record. It shall have no
other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">delegate</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)
* `representative` (representative's account name)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to assign a reperesentative for the data records owner. The
representative can modify the records and claim refund. It shall have no
other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">modtokenname</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)
* `oldtokenname` (token name or emty string)
* `newtokenname` (token name or emty string)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to modify the token name of a specified data record. It shall have no
other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">remvoucher</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to revoke a previously issued discount voucher from an entry owner. It
shall have no other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">setattr</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)
* `tokenname` (token name or emty string)
* `key` (key name, up to 12 symbols)
* `value` (value string, up to 80 characters)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to set an attribute for a specified data record. It shall have no other
effect. Up to 5 attributes are allowed per entry.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">setflag</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)
* `tokenname` (token name or emty string)
* `flag` (flag name, one of (ready, complete, incomplete, show, hide)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to modify the status of a specified data record. It shall have no other
effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">setprice</h1>

### Parameters

Input parameters:

* `contract` (token contract name)
* `price_newentry` (standard price for the first entry in the catalog)
* `price_subentry` (standard price for additional entry for the same owner)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to set prices for new entries. It shall have no other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">settags</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)
* `tokenname` (token name or emty string)
* `tags` (array of tag names)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to replace tags assigned to a specified data record. It shall have no
other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">setvalue</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)
* `tokenname` (token name or emty string)
* `key` (record field name, such as title, org, urlwebsite, urllogo, urlsrccode, email, descr)
* `value` (string value for a field, up to 1024 characters)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to modify a specified data record. It shall have no other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">setvalues</h1>

### Parameters

Input parameters:

* `owner` (entry owner's account name)
* `tokenname` (token name or emty string)
* `values` (array of (key,value) pairs. Key names are the same as in setvalue action)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to modify a specified data record. It shall have no other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">startpromo</h1>

### Parameters

Input parameters:

* `contract` (token contract name)
* `price` (promotion price for the first entry in the catalog)
* `count` (number of new entries offered at promotion price)


### Intent

INTENT. The intention of the author and the invoker of this contract is
to set prices for new entries during a promotion action. It shall have
no other effect.

### Term

TERM. This Contract expires at the conclusion of code execution.
