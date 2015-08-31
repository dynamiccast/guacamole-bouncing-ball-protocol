# guacamole-bouncing-ball-protocol
Contains a working example of the bouncing ball protocol implemented in Guacamole's guide as an example to implement custom protocols for guacd.

# Build and install

1. Ensure guacd is installed
2. Run `autoreconf -fi` to generate ./configure file
3. Run `./configure` to generate Makefile
4. Run `make` to build protocol
5. Run `make install` to install the bouncing ball protocol along with guacd other protocols
 
# Configure protocol

Now the bouncing ball protocol is installed, we need to configure a new connection in `user-mapping.xml` (by default in `/etc/guacamole/user-mapping.xml`).
Just add the following :

```xml
<connection name="Ball tutorial">
  <protocol>ball</protocol>
</connection>

```
