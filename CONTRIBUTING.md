# Contributing to Robot Car Arduino

Thank you for your interest in contributing to Robot Car Arduino! This document provides guidelines and instructions for contributing to the project.

---

## 🤝 Code of Conduct

We are committed to providing a welcoming and inspiring community for all. By participating in this project, you agree to uphold our Code of Conduct:

- **Be Respectful:** Treat everyone with respect and kindness
- **Be Inclusive:** Welcome diverse perspectives and backgrounds
- **Be Professional:** Maintain professional communication
- **Be Collaborative:** Work together toward common goals
- **Be Transparent:** Be honest about limitations and uncertainties

---

## 📋 Getting Started

### Prerequisites

- Familiarity with C/C++ programming
- Basic understanding of Arduino development
- Knowledge of embedded systems (recommended)
- Git version control basics

### Development Environment Setup

1. **Clone Repository:**
   ```bash
   git clone https://github.com/Crypt0xDev/robot-car-arduino.git
   cd robot-car-arduino
   ```

2. **Install Arduino IDE:**
   - Download from [arduino.cc/software](https://www.arduino.cc/en/software)
   - Install Arduino AVR Boards package

3. **Install Dependencies:**
   - Servo library (built-in)
   - SoftwareSerial (built-in)
   - Additional libraries via Arduino IDE

4. **Verify Setup:**
   ```bash
   # Open Arduino IDE and compile example sketches
   # Verify all tests pass
   ```

---

## 🎯 Types of Contributions

### 1. Bug Reports

**Found a bug?** Report it on [GitHub Issues](../../issues)

**Before submitting:**
- [ ] Check existing issues (may be already reported)
- [ ] Reproduce with latest code
- [ ] Collect error messages and logs

**Include in report:**
- Environment (OS, Arduino IDE version)
- Hardware configuration
- Steps to reproduce
- Expected vs actual behavior
- Screenshots/videos (if applicable)

### 2. Feature Requests

**Have an idea?** Suggest it on [GitHub Discussions](../../discussions)

**Proposal should include:**
- Clear description of feature
- Use cases and benefits
- Potential implementation approach
- Impact on existing code
- Any new dependencies required

### 3. Code Contributions

**Want to code?** Follow the process below

### 4. Documentation

**Improve documentation:**
- Fix typos or unclear sections
- Add missing information
- Create tutorials or guides
- Improve examples

### 5. Testing

**Help with testing:**
- Test releases before official launch
- Report edge cases
- Stress test components
- Verify across platforms

---

## 🔧 Development Workflow

### Step 1: Fork & Clone

```bash
# Fork on GitHub (click Fork button)
git clone https://github.com/YOUR_USERNAME/robot-car-arduino.git
cd robot-car-arduino
```

### Step 2: Create Feature Branch

```bash
# Create branch from main
git checkout -b feature/YourFeatureName

# Or for bug fixes
git checkout -b fix/BugDescription

# Or for documentation
git checkout -b docs/DocumentationTopic
```

**Branch Naming Convention:**
- `feature/` - New features
- `fix/` - Bug fixes
- `docs/` - Documentation
- `refactor/` - Code refactoring
- `test/` - Test additions

### Step 3: Make Changes

**Code Style Guidelines:**

```cpp
// Use descriptive names
int motorSpeed = 150;  // ✅ Good
int ms = 150;           // ❌ Avoid

// Comment complex logic
// Scan servo 0° to 180° and find clearest path
void scanEnvironment() {
  // ...
}

// Keep functions focused (single responsibility)
void moveForward(int speed);   // ✅ Single task
void moveAndTurn(int s, int a); // ❌ Multiple tasks

// Consistent formatting
void function() {
  if (condition) {
    // code
  }
}

// Use constants for magic numbers
#define OBSTACLE_THRESHOLD 20  // ✅ Clear meaning
if (distance < 20) {}           // ❌ Magic number
```

**File Organization:**

```
YourFeature/
├── YourFeature.h           # Header with interface
├── YourFeature.cpp         # Implementation
├── README.md               # Documentation
└── test_yourfeature.cpp    # Test cases
```

### Step 4: Test Your Changes

**Write Tests:**
```cpp
void test_your_feature() {
  // Arrange
  YourFeature feature;

  // Act
  feature.doSomething();

  // Assert
  assert(feature.getResult() == expected);
}
```

**Run Tests:**
- Compile without errors
- All existing tests pass
- New feature tests pass
- Hardware testing (if applicable)

### Step 5: Commit Changes

**Commit Message Format:**
```
[Type]: Brief description (50 chars max)

Detailed explanation if needed. Wrap at 72 characters.
Explain WHAT and WHY, not HOW.

- Use bullet points for multiple changes
- Reference issues: Fixes #123, Related to #456

Examples:
[feature]: Add PID controller for motor speed
[fix]: Correct ultrasonic measurement calculation
[docs]: Update wiring guide with new pinout
[refactor]: Simplify obstacle avoidance logic
```

**Commit Guidelines:**
```bash
# Make logical commits
git add src/feature/MyFeature.h
git commit -m "[feature]: Add MyFeature class"

git add test/test_myfeature.cpp
git commit -m "[test]: Add MyFeature unit tests"

# Don't do too much in one commit
git add .               # ❌ Avoid
git commit -m "Various changes"
```

### Step 6: Push & Create Pull Request

```bash
# Push to your fork
git push origin feature/YourFeatureName

# Create PR on GitHub
# - Write clear PR description
# - Reference related issues
# - List changes made
# - Include testing notes
```

**Pull Request Template:**

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] New feature
- [ ] Bug fix
- [ ] Documentation update
- [ ] Code refactoring

## Related Issues
Fixes #123
Related to #456

## Changes Made
- Change 1
- Change 2

## Testing
Describe testing performed

## Screenshots/Videos
(if applicable)

## Checklist
- [ ] Code follows style guidelines
- [ ] Tests added/updated
- [ ] Documentation updated
- [ ] No breaking changes
- [ ] Tested on hardware
```

### Step 7: Code Review

**What to expect:**
- Code review by maintainers
- Requests for changes (if needed)
- Constructive feedback
- Discussion about approach

**Tips for review process:**
- Respond promptly to comments
- Ask clarifying questions
- Be open to suggestions
- Make requested changes

### Step 8: Merge & Deploy

Once approved:
- PR is merged to main
- Changes deployed in next release
- Contributor credited
- Feature documented

---

## 📝 Coding Standards

### Naming Convention

| Type | Style | Example |
|------|-------|---------|
| **Classes** | PascalCase | `MotorDriver` |
| **Functions** | camelCase | `getDistance()` |
| **Variables** | camelCase | `motorSpeed` |
| **Constants** | UPPER_CASE | `MAX_SPEED` |
| **Private** | _underscore | `_internalValue` |
| **Booleans** | is/has prefix | `isMoving`, `hasError` |

### File Organization

```cpp
// YourModule.h
#ifndef YOUR_MODULE_H
#define YOUR_MODULE_H

#include <Arduino.h>
#include "../Config/PinDefinitions.h"

class YourModule {
  public:
    YourModule();
    void init();
    void update();

  private:
    void _internalMethod();
    int _internalValue;
};

#endif
```

### Documentation

```cpp
/**
 * @brief Brief description
 * @param param1 Description of parameter
 * @return Description of return value
 * @note Any important notes
 * @example
 *   YourClass obj;
 *   obj.doSomething();
 */
void doSomething(int param1);
```

---

## 🧪 Testing Requirements

### For New Features

- [ ] Unit tests written
- [ ] Integration tests pass
- [ ] Edge cases handled
- [ ] Error handling verified
- [ ] Performance acceptable
- [ ] Memory usage acceptable

### For Bug Fixes

- [ ] Root cause identified
- [ ] Fix verified to work
- [ ] Regression tests added
- [ ] No new issues introduced
- [ ] Performance not degraded

### Testing Tools

```cpp
// Simple test framework
void test_my_feature() {
  Serial.println("Testing my feature...");

  // Setup
  MyFeature feature;

  // Test
  bool result = feature.doSomething();

  // Verify
  if (result) {
    Serial.println("✓ PASS");
  } else {
    Serial.println("✗ FAIL");
  }
}

// In main loop
void loop() {
  test_my_feature();
  delay(5000);
}
```

---

## 📚 Documentation Requirements

### For Code Changes

- [ ] Comments explaining complex logic
- [ ] Function documentation
- [ ] Class documentation
- [ ] Usage examples
- [ ] Parameter descriptions

### For New Features

- [ ] README.md in feature directory
- [ ] Updated main documentation
- [ ] Added to roadmap/changelog
- [ ] Tutorial or usage guide
- [ ] API documentation

### Documentation Template

```markdown
# Feature Name

## Overview
Brief description of feature

## API Reference
```cpp
// Code examples
```

## Usage Example
Step-by-step usage

## Configuration
Any configuration options

## Troubleshooting
Common issues and solutions

## References
Links to related docs
```

---

## 🐛 Bug Fix Process

1. **Verify Bug** - Reproduce the issue
2. **Root Cause** - Identify why it happens
3. **Solution** - Fix the problem
4. **Test Fix** - Verify it works
5. **Regression Test** - Ensure no new issues
6. **Document** - Explain the fix
7. **Submit PR** - Create pull request

---

## 🚀 Feature Implementation Process

1. **Proposal** - Discuss in GitHub Issues
2. **Design** - Plan the implementation
3. **Implement** - Write the code
4. **Test** - Comprehensive testing
5. **Document** - Create usage docs
6. **Review** - Code review process
7. **Deploy** - Release in next version

---

## 🔄 Project Governance

### Maintainers

- **Owner:** Alexis Noe Gonzales Perez
- **Lead Maintainer:** [Role]
- **Maintainers:** [List]

### Decision Making

- Small changes: Merged after review
- Large features: Discussed in Issues first
- Breaking changes: RFC (Request for Comments)
- Security issues: Private disclosure

### Release Cycle

- **Major (x.0.0):** Quarterly or when ready
- **Minor (x.y.0):** Bi-monthly
- **Patch (x.y.z):** As needed (bugs)

---

## 📞 Getting Help

### Questions About Contributing?

- **Documentation:** Check [docs/](../docs/)
- **Issues:** Check [existing issues](../../issues)
- **Discussions:** [GitHub Discussions](../../discussions)
- **Email:** contact@example.com

### Resources

- [Git/GitHub Guide](https://guides.github.com/)
- [Arduino Development](https://www.arduino.cc/en/Tutorial)
- [C++ Best Practices](https://isocpp.org/)

---

## 🎁 Recognition

Contributors will be recognized in:
- README.md contributors section
- Release notes
- Project website
- Annual contributors list

---

## 📜 License

By contributing to Robot Car Arduino, you agree that your contributions will be licensed under the MIT License.

---

## ✅ Contribution Checklist

Before submitting PR:

- [ ] Code follows project style guidelines
- [ ] Self-review completed
- [ ] Comments added for complex logic
- [ ] Documentation updated
- [ ] Tests added/updated
- [ ] All tests pass
- [ ] No new warnings introduced
- [ ] No breaking changes
- [ ] Commit messages are clear
- [ ] PR description is complete

---

**Thank you for contributing to Robot Car Arduino!** 🚀

Your contributions help make robotics more accessible and inspiring for everyone.
