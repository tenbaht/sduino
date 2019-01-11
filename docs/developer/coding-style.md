# Coding style

## Development workflow

I recently read this great article about [a successful branching
model](https://nvie.com/posts/a-successful-git-branching-model/) for git and
will implement it. Highly recommended!


## Changelog

Every feature branch (or pull request) should include an update for
Changelog.md


## Code format

When porting an existing Arduino library I try to preserve the original
formatting as much as possible to allow for easy comparison to the original
state.

There is a development of a C++ capable compiler environment going on. There
is no public website for that project yet, but I am in contact with the
developer. He is even targeting Sduino for his project and at one point in
the (still distant) future we might be able to switch back to the orginal
library code again. Preserved code structure would be a big plus then. (But
don't hold your breath yet)

Newly written code is mostly formatted using indent according to the linux
kernel coding style, using Tabs, Tab width 8, line length 80:

```bash
indent -linux
```
